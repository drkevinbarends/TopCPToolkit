from AnalysisAlgorithmsConfig.ConfigBlock import ConfigBlock


class EventShapeConfig(ConfigBlock):
    """ConfigBlock for adding EventShape"""

    def __init__(self):
        super(EventShapeConfig, self).__init__()
        self.addOption("eventShape", "Kt4EMPFlowNeutEventShape", type=str)

    def makeAlgs(self, config):
        alg = config.createAlgorithm(
            "top::EventShape",
            "EventShape",
        )

        alg.eventShape = self.eventShape

        alg.affectingSystematicsFilter = ".*"

        alg.density = "event_shape_density_%SYS%"

        config.addOutputVar("EventInfo", alg.density, "event_shape_density")
