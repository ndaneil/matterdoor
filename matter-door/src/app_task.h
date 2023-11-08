/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#pragma once

#include "app_event.h"
#include "led_widget.h"

#include <platform/CHIPDeviceLayer.h>

#if CONFIG_CHIP_FACTORY_DATA
#include <platform/nrfconnect/FactoryDataProvider.h>
#else
#include <platform/nrfconnect/DeviceInstanceInfoProviderImpl.h>
#endif

#include <app/clusters/identify-server/identify-server.h>

#include <app/clusters/on-off-server/on-off-server.h>
#include <app/clusters/level-control/level-control.h>

struct k_timer;

class AppTask {
public:
	static AppTask &Instance()
	{
		static AppTask sAppTask;
		return sAppTask;
	};

	
	enum DispState {
		OFF = 0,
		FREE = 1,
		BUSY = 2
	};

	CHIP_ERROR StartApp();

	static void PostEvent(const AppEvent &event);
	void setState(DispState newState);
	static void dispThread(void);
	DispState currentState = OFF;


private:
	CHIP_ERROR Init();

	void CancelTimer();
	void StartTimer(uint32_t timeoutInMs);

	static void DispatchEvent(const AppEvent &event);
	static void UpdateLedStateEventHandler(const AppEvent &event);
	static void FunctionHandler(const AppEvent &event);
	static void FunctionTimerEventHandler(const AppEvent &event);

	static void ChipEventHandler(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg);
	static void ButtonEventHandler(uint32_t buttonState, uint32_t hasChanged);
	static void LEDStateUpdateHandler(LEDWidget &ledWidget);
	static void FunctionTimerTimeoutCallback(k_timer *timer);
	static void UpdateStatusLED();

	static void IdentifyStartHandler(Identify *identify);
	static void IdentifyStopHandler(Identify *identify);

	static void OnOffEffectHandler(OnOffEffect * effect);
	static void clearAll();
	static void set(int x, int y, int r, int g, int b);
	static void drawX(int offset100, int brightness);
	static void drawFree(int offset100, int brightness);

	FunctionEvent mFunction = FunctionEvent::NoneSelected;
	bool mFunctionTimerActive = false;

	static Identify sIdentify;
	static OnOffEffect sOnOffEffect;


#if CONFIG_CHIP_FACTORY_DATA
	chip::DeviceLayer::FactoryDataProvider<chip::DeviceLayer::InternalFlashFactoryData> mFactoryDataProvider;
#endif
};
