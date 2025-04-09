# Contributing

TopCPToolkit needs to adjust to the needs of a growing basis of users, but with a limited number of maintainers.
Feedback, bug fixes and improvements from the community are therefore very welcome!

## How to contribute

Your first port of call is the [issue tracker](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues).
There you can communicate with the developer team, and make changes happen.

### Create an issue

:material-bug: **Something is not working?** [:octicons-arrow-right-24: Report a bug](reporting_bug.md)

:material-lightbulb-on: **Thought of something we can add?** [:octicons-arrow-right-24: Request a feature](requesting_feature.md)

:material-plus-network-outline: **Want to add your own changes?** [:octicons-arrow-right-24: Propose an algorithm](proposing_algorithm.md)

:material-arrow-up-circle: **Anything else we can improve?** [:octicons-arrow-right-24: Suggest an improvement](suggesting_improvement.md)

### Path to a new release

!!! warning
    For the core maintainer team only!

Releases are labeled as `vX.Y.Z`:

- very minor updates increase the value of `Z` (e.g. changes to the CI, docs, quick bugfix)
- regular (bi/weekly) updates increase the value of `Y` (e.g. most user requests and dev work)
- only major updates warrant an increase of `X` (e.g. significant changes to the framework, conceptual differences that need to be carefully implemented by analysers)

To create a new release, please follow these steps:

1. Update the [changelog](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/blob/main/docs/changelog/index.md) with information about the changes in the `main` branch that will make it to the new release
    - make a note of which AnalysisBase version is needed to run
    - put today's date
    - you can also update the list of open issues under "upcoming release"
    - updating the changelog can be done by pushing directly to `main`, no need for a MR
1. Go to "Settings", then "General": [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/edit). Move down to "Badges", click "Expand". Edit the badge named `ABrelease`: under "Badge image URL", update the version of AnalysisBase as appropriate.
1. Go to "Code", then "Tags": [here](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/tags). Click the blue button "New tag".
    - under "Tag name", put `vX.Y.Z` as appropriate
    - ensure that "Create from" points to `main`
    - in "Message", copy paste exactly what you put in the changelog under the date/version header
    - click "Create tag": this will also trigger a new pipeline that will convert this new tag into a numbered release
1. Check on the [homepage](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit) that the "Latest Release" badge has been updated correctly.
1. Inform the Mattermost channel of the new release!