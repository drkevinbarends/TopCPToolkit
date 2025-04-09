New algorithms are very much welcome! You can propose them by submitting an issue in our [issue tracker](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues) and following this guide.

## Before creating an issue

### Upgrade to the latest version

Please make sure that you use the latest versions of TopCPTookit and AnalysisBase, so that your new algorithm can be beneficial to most other users.

### Make it customizable

Use read- and write-handles as much as possible, expose all properties in the python config block and set reasonable defaults. As a rule of thumb, no single property or object collection in the C++ code should be hardcoded, and from this restriction the available options should naturally follow!

### Document your work

We'd appreciate your expert input to our docs, but will in any case iterate with you on this.

## I read the above...

Then please click on the button below and use the appropriate template :thumbsup:

[:material-vote: Open an issue](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/new){ .md-button .md-button--primary }