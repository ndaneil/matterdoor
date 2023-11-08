/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "app_task.h"
#include "app_config.h"
#include "led_util.h"

#include <platform/CHIPDeviceLayer.h>

#include "board_util.h"
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CodeUtils.h>
#include <system/SystemError.h>

#ifdef CONFIG_CHIP_WIFI
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <platform/nrfconnect/wifi/NrfWiFiDriver.h>
#endif

#ifdef CONFIG_CHIP_OTA_REQUESTOR
#include "ota_util.h"
#endif

#include <dk_buttons_and_leds.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <app-common/zap-generated/attributes/Accessors.h>

#include <errno.h>
#include <string.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/sys/util.h>

#include <zephyr/sys/printk.h>

#define STRIP_NODE		DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)

#define DELAY_TIME K_MSEC(6)

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }

#define DISPLAY_TASK_PRIORITY 1

static struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

K_THREAD_DEFINE(disp_thread, 2048, AppTask::dispThread, NULL, NULL, NULL, DISPLAY_TASK_PRIORITY, 0, 0);


void AppTask::set(int x, int y, int r, int g, int b){
	int idx = 0;
	if(y%2 == 0){
		idx = y*12+(11-x);
	}else{
		idx = y*12+x;
	}
	pixels[idx].r = r;
	pixels[idx].g = g;
	pixels[idx].b = b;
}

void AppTask::clearAll(){
	memset(&pixels, 0x00, sizeof(pixels));
}

int inline flicker(int value, int offset){
	return (value*(100-abs(50-offset%100)))/100;
}

void AppTask::drawX(int offset100, int brightness){
	int factor = offset100%100;
	int offset = offset100/100;
	clearAll();

	set(4,1,flicker(brightness,factor),0,0);
	set(5,1,flicker(brightness,factor),0,0);
	set(6,1,flicker(brightness,factor),0,0);
	set(7,1,flicker(brightness,factor),0,0);

	set(4,2,flicker(brightness,factor+20),0,0);
	set(7,2,flicker(brightness,factor+20),0,0);

	set(4,3,flicker(brightness,factor+40),0,0);
	set(7,3,flicker(brightness,factor+40),0,0);

	set(4,3,flicker(brightness,factor+60),0,0);
	set(5,3,flicker(brightness,factor+60),0,0);
	set(6,3,flicker(brightness,factor+60),0,0);
	set(7,3,flicker(brightness,factor+60),0,0);
	
	

	int level = flicker(brightness,offset*9+factor/12)-brightness/3;
	for(int i = 0; i < 5; i++){
		set(0,i,level,brightness/10,0);
		set(11,i,level,brightness/10,0);
	}	
}

void AppTask::drawFree(int offset100, int brightness){
	int factor = offset100%100;
	int offset = (offset100/240)%5;
	clearAll();

	int fromzero = 120-abs(120-offset100%240);
	int rcolor = (brightness*fromzero)/120;

	set(0,0,0,flicker(brightness,factor),0);
	set(11,0,0,flicker(brightness,factor),0);
	
	set(0,1,0,flicker(brightness,factor+20),0);
	set(11,1,0,flicker(brightness,factor+20),0);
	
	set(0,2,0,flicker(brightness,factor+40),0);
	set(11,2,0,flicker(brightness,factor+40),0);
	
	set(0,3,0,flicker(brightness,factor+60),0);
	set(11,3,0,flicker(brightness,factor+60),0);
	
	set(0,4,0,flicker(brightness,factor+80),0);
	set(11,4,0,flicker(brightness,factor+80),0);
	
	for(int i = 3; i < 9; i++){
		set(i,offset,0,rcolor,0);
	}
}


LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;

namespace
{
constexpr size_t kAppEventQueueSize = 10;
constexpr uint32_t kFactoryResetTriggerTimeout = 6000;
constexpr uint8_t kLightEndpointId = 1;

K_MSGQ_DEFINE(sAppEventQueue, sizeof(AppEvent), kAppEventQueueSize, alignof(AppEvent));
k_timer sFunctionTimer;

LEDWidget sStatusLED;
#if NUMBER_OF_LEDS == 2
FactoryResetLEDsWrapper<1> sFactoryResetLEDs{ { FACTORY_RESET_SIGNAL_LED } };
#else
FactoryResetLEDsWrapper<3> sFactoryResetLEDs{ { FACTORY_RESET_SIGNAL_LED, FACTORY_RESET_SIGNAL_LED1,
						FACTORY_RESET_SIGNAL_LED2 } };
#endif

bool sIsNetworkProvisioned = false;
bool sIsNetworkEnabled = false;
bool sHaveBLEConnections = false;
} /* namespace */

namespace LedConsts
{
namespace StatusLed
{
	namespace Unprovisioned
	{
		constexpr uint32_t kOn_ms{ 100 };
		constexpr uint32_t kOff_ms{ kOn_ms };
	} /* namespace Unprovisioned */
	namespace Provisioned
	{
		constexpr uint32_t kOn_ms{ 50 };
		constexpr uint32_t kOff_ms{ 950 };
	} /* namespace Provisioned */

} /* namespace StatusLed */
	constexpr uint32_t kIndentifyBlinkRate_ms{ 500 };
} /* namespace LedConsts */

#ifdef CONFIG_CHIP_WIFI
app::Clusters::NetworkCommissioning::Instance
	sWiFiCommissioningInstance(0, &(NetworkCommissioning::NrfWiFiDriver::Instance()));
#endif

Identify AppTask::sIdentify = { kLightEndpointId, AppTask::IdentifyStartHandler, AppTask::IdentifyStopHandler, EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LED};
OnOffEffect AppTask::sOnOffEffect = { kLightEndpointId, AppTask::OnOffEffectHandler };


CHIP_ERROR AppTask::Init()
{
	/* Initialize CHIP stack */
	LOG_INF("Init CHIP stack");

	CHIP_ERROR err = chip::Platform::MemoryInit();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("Platform::MemoryInit() failed");
		return err;
	}

	err = PlatformMgr().InitChipStack();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().InitChipStack() failed");
		return err;
	}

#if defined(CONFIG_NET_L2_OPENTHREAD)
	err = ThreadStackMgr().InitThreadStack();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("ThreadStackMgr().InitThreadStack() failed: %s", ErrorStr(err));
		return err;
	}

#ifdef CONFIG_OPENTHREAD_MTD_SED
	err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_SleepyEndDevice);
#elif CONFIG_OPENTHREAD_MTD
	err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_MinimalEndDevice);
#else
	err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_Router);
#endif /* CONFIG_OPENTHREAD_MTD_SED */
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("ConnectivityMgr().SetThreadDeviceType() failed: %s", ErrorStr(err));
		return err;
	}

#elif defined(CONFIG_CHIP_WIFI)
	sWiFiCommissioningInstance.Init();
#else
	return CHIP_ERROR_INTERNAL;
#endif /* CONFIG_NET_L2_OPENTHREAD */

	/* Initialize LEDs */
	LEDWidget::InitGpio();
	LEDWidget::SetStateUpdateCallback(LEDStateUpdateHandler);

	sStatusLED.Init(SYSTEM_STATE_LED);

	UpdateStatusLED();

	/* Initialize buttons */
	int ret = dk_buttons_init(ButtonEventHandler);
	if (ret) {
		LOG_ERR("dk_buttons_init() failed");
		return chip::System::MapErrorZephyr(ret);
	}

	/* Initialize function timer */
	k_timer_init(&sFunctionTimer, &AppTask::FunctionTimerTimeoutCallback, nullptr);
	k_timer_user_data_set(&sFunctionTimer, this);

	/* Initialize CHIP server */
#if CONFIG_CHIP_FACTORY_DATA
	ReturnErrorOnFailure(mFactoryDataProvider.Init());
	SetDeviceInstanceInfoProvider(&mFactoryDataProvider);
	SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);
	SetCommissionableDataProvider(&mFactoryDataProvider);
#else
	SetDeviceInstanceInfoProvider(&DeviceInstanceInfoProviderMgrImpl());
	SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

	static chip::CommonCaseDeviceServerInitParams initParams;
	(void)initParams.InitializeStaticResourcesBeforeServerInit();

	ReturnErrorOnFailure(chip::Server::GetInstance().Init(initParams));
	ConfigurationMgr().LogDeviceConfig();
	PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));

	/*
	 * Add CHIP event handler and start CHIP thread.
	 * Note that all the initialization code should happen prior to this point to avoid data races
	 * between the main and the CHIP threads.
	 */
	PlatformMgr().AddEventHandler(ChipEventHandler, 0);

	err = PlatformMgr().StartEventLoopTask();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().StartEventLoopTask() failed");
		return err;
	}


	return CHIP_NO_ERROR;
}



CHIP_ERROR AppTask::StartApp()
{
	ReturnErrorOnFailure(Init());

	AppEvent event = {};

	while (true) {
		k_msgq_get(&sAppEventQueue, &event, K_FOREVER);
		DispatchEvent(event);
	}

	return CHIP_NO_ERROR;
}

void AppTask::ButtonEventHandler(uint32_t buttonState, uint32_t hasChanged)
{
	AppEvent button_event;
	button_event.Type = AppEventType::Button;

	if (FUNCTION_BUTTON_MASK & hasChanged) {
		button_event.ButtonEvent.PinNo = FUNCTION_BUTTON;
		button_event.ButtonEvent.Action =
			static_cast<uint8_t>((FUNCTION_BUTTON_MASK & buttonState) ? AppEventType::ButtonPushed :
										    AppEventType::ButtonReleased);
		button_event.Handler = FunctionHandler;
		PostEvent(button_event);
	}
}

void AppTask::FunctionTimerTimeoutCallback(k_timer *timer)
{
	if (!timer) {
		return;
	}

	AppEvent event;
	event.Type = AppEventType::Timer;
	event.TimerEvent.Context = k_timer_user_data_get(timer);
	event.Handler = FunctionTimerEventHandler;
	PostEvent(event);
}

void AppTask::FunctionTimerEventHandler(const AppEvent &)
{
	if (Instance().mFunction == FunctionEvent::FactoryReset) {
		Instance().mFunction = FunctionEvent::NoneSelected;
		LOG_INF("Factory Reset triggered");

		sStatusLED.Set(true);
		sFactoryResetLEDs.Set(true);

		chip::Server::GetInstance().ScheduleFactoryReset();
	}
}

void AppTask::FunctionHandler(const AppEvent &event)
{
	if (event.ButtonEvent.PinNo != FUNCTION_BUTTON)
		return;

	if (event.ButtonEvent.Action == static_cast<uint8_t>(AppEventType::ButtonPushed)) {
		Instance().StartTimer(kFactoryResetTriggerTimeout);
		Instance().mFunction = FunctionEvent::FactoryReset;
	} else if (event.ButtonEvent.Action == static_cast<uint8_t>(AppEventType::ButtonReleased)) {
		if (Instance().mFunction == FunctionEvent::FactoryReset) {
			sFactoryResetLEDs.Set(false);
			UpdateStatusLED();
			Instance().CancelTimer();
			Instance().mFunction = FunctionEvent::NoneSelected;
			LOG_INF("Factory Reset has been Canceled");
		}
	}
}

void AppTask::LEDStateUpdateHandler(LEDWidget &ledWidget)
{
	AppEvent event;
	event.Type = AppEventType::UpdateLedState;
	event.Handler = UpdateLedStateEventHandler;
	event.UpdateLedStateEvent.LedWidget = &ledWidget;
	PostEvent(event);
}

void AppTask::UpdateLedStateEventHandler(const AppEvent &event)
{
	if (event.Type == AppEventType::UpdateLedState) {
		event.UpdateLedStateEvent.LedWidget->UpdateState();
	}
}

void AppTask::UpdateStatusLED()
{
	/* Update the status LED.
	 *
	 * If IPv6 networking and service provisioned, keep the LED On constantly.
	 *
	 * If the system has BLE connection(s) uptill the stage above, THEN blink the LED at an even
	 * rate of 100ms.
	 *
	 * Otherwise, blink the LED for a very short time. */
	if (sIsNetworkProvisioned && sIsNetworkEnabled) {
		sStatusLED.Set(true);
	} else if (sHaveBLEConnections) {
		sStatusLED.Blink(LedConsts::StatusLed::Unprovisioned::kOn_ms,
				 LedConsts::StatusLed::Unprovisioned::kOff_ms);
	} else {
		sStatusLED.Blink(LedConsts::StatusLed::Provisioned::kOn_ms, LedConsts::StatusLed::Provisioned::kOff_ms);
	}
}

void AppTask::ChipEventHandler(const ChipDeviceEvent *event, intptr_t /* arg */)
{
	switch (event->Type) {
	case DeviceEventType::kCHIPoBLEAdvertisingChange:
		sHaveBLEConnections = ConnectivityMgr().NumBLEConnections() != 0;
		UpdateStatusLED();
		break;
#if defined(CONFIG_NET_L2_OPENTHREAD)
	case DeviceEventType::kDnssdInitialized:
#if CONFIG_CHIP_OTA_REQUESTOR
		InitBasicOTARequestor();
#endif /* CONFIG_CHIP_OTA_REQUESTOR */
		break;
	case DeviceEventType::kThreadStateChange:
		sIsNetworkProvisioned = ConnectivityMgr().IsThreadProvisioned();
		sIsNetworkEnabled = ConnectivityMgr().IsThreadEnabled();
#elif defined(CONFIG_CHIP_WIFI)
	case DeviceEventType::kWiFiConnectivityChange:
		sIsNetworkProvisioned = ConnectivityMgr().IsWiFiStationProvisioned();
		sIsNetworkEnabled = ConnectivityMgr().IsWiFiStationEnabled();
#if CONFIG_CHIP_OTA_REQUESTOR
		if (event->WiFiConnectivityChange.Result == kConnectivity_Established) {
			InitBasicOTARequestor();
		}
#endif /* CONFIG_CHIP_OTA_REQUESTOR */
#endif
		UpdateStatusLED();
		break;
	default:
		break;
	}
}

void AppTask::CancelTimer()
{
	k_timer_stop(&sFunctionTimer);
}

void AppTask::StartTimer(uint32_t timeoutInMs)
{
	k_timer_start(&sFunctionTimer, K_MSEC(timeoutInMs), K_NO_WAIT);
}

void AppTask::PostEvent(const AppEvent &event)
{
	if (k_msgq_put(&sAppEventQueue, &event, K_NO_WAIT) != 0) {
		LOG_INF("Failed to post event to app task event queue");
	}
}

void AppTask::DispatchEvent(const AppEvent &event)
{
	if (event.Handler) {
		event.Handler(event);
	} else {
		LOG_INF("Event received with no handler. Dropping event.");
	}
}

void AppTask::IdentifyStartHandler(Identify *) {
	AppEvent event;
	event.Type = AppEventType::IdenfityStart;
	event.Handler = [](const AppEvent &event) {
		sFactoryResetLEDs.Blink(LedConsts::kIndentifyBlinkRate_ms);
	};
	PostEvent(event);
}
void AppTask::IdentifyStopHandler(Identify *) {
	AppEvent event;
	event.Type = AppEventType::IdentifyStop;
	event.Handler = [](const AppEvent &event) {
		sFactoryResetLEDs.Set(false);
	};
	PostEvent(event);
}
void AppTask::OnOffEffectHandler(OnOffEffect *effect) {
	//TODO....
}

void AppTask::setState(DispState newState){
	currentState = newState;
}

void AppTask::dispThread(void){
	int rc;
	printk("Display thread started\n");
	if (device_is_ready(strip)) {
		LOG_INF("Found LED strip device %s", strip->name);
	} else {
		LOG_ERR("LED strip device %s is not ready", strip->name);
		return;
	}

	LOG_INF("Displaying pattern on strip");
	memset(&pixels, 0x00, sizeof(pixels));
	DispState prevState = Instance().currentState;
	DispState nextState = Instance().currentState;
	int maxbrightness = 50;
	int cntr = 0;
	int currbrightness = 50;

	while (1) {
		if(nextState != prevState){
			if(currbrightness <= 0){
				prevState = nextState;
				currbrightness++;
			}else{
				currbrightness--;
			}
		}else if(currbrightness < maxbrightness){
			currbrightness++;
		}

		cntr = (cntr+1)%(100*12);
		switch (prevState){
		case OFF:
			clearAll();
			break;
		case FREE:
			drawFree(cntr,currbrightness);
			break;
		case BUSY:
			drawX(cntr,currbrightness);
			break;
		default:
			break;
		}
		rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
		if (rc) {
			LOG_ERR("couldn't update strip: %d", rc);
		}
		k_sleep(DELAY_TIME);
		if(nextState != Instance().currentState){
			nextState = Instance().currentState;
		}
	}
}