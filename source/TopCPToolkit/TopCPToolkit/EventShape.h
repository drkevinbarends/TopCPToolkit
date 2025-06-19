#ifndef EVENT_SHAPE_H
#define EVENT_SHAPE_H

// Algorithm includes
#include <AnaAlgorithm/AnaAlgorithm.h>
#include <SelectionHelpers/SysReadSelectionHandle.h>
#include <SystematicsHandles/SysReadDecorHandle.h>
#include <SystematicsHandles/SysReadHandle.h>
#include <SystematicsHandles/SysWriteDecorHandle.h>
#include <SystematicsHandles/SysWriteHandle.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODEventShape/EventShape.h>

namespace top {

class EventShape final : public EL::AnaAlgorithm {

 public:
  EventShape(const std::string &name, ISvcLocator *pSvcLocator);
  virtual StatusCode initialize() override final;
  virtual StatusCode execute() override final;
  virtual StatusCode finalize() override final;

 private:
  // systematics
  CP::SysListHandle m_systematicsList{this};

  CP::SysReadHandle<xAOD::EventInfo> m_eventInfoHandle{
      this, "eventInfo", "EventInfo",
      "the EventInfo container to read selection decisions from"};

  CP::SysReadHandle<xAOD::EventShape> m_eventShapeHandle{
      this, "eventShape", "", "the EventShape container to read from"};

  CP::SysWriteDecorHandle<double> m_density_decor{
      this, "density", "event_shape_density_%SYS%", "average event density"};
};
}  // namespace top

#endif
