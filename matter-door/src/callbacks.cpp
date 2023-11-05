#include "app_task.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>

#include <app/data-model/Nullable.h>
#include <lib/core/DataModelTypes.h>
#include <lib/support/CodeUtils.h>


#include <app/clusters/on-off-server/on-off-server.h>
#include <app/clusters/level-control/level-control.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::ColorControl;
using namespace ::chip::app::Clusters::LevelControl;
using namespace ::chip::app::Clusters::OnOff;
using ::chip::app::DataModel::Nullable;

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{

    LOG_INF("-----------------> Post attribute change callback! %d %d %d", type, size, *value);

    if(type == 32){
        if(size == 1){
            LOG_INF("Setting display state to %d", *value);
            AppTask::Instance().setState((AppTask::DispState) (*value));
        }
    }
}
