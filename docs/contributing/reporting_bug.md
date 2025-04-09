If you have discovered a bug, you can help us by submitting an issue in our [issue tracker](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues) and following this guide.

## Before creating an issue

### Upgrade to the latest version

Chances are that the bug you discovered was already fixed in a subsequent
version.
Thus, before reporting an issue, please ensure that you're running the latest version of TopCPToolkit and AnalysisBase.

!!! warning "Bug fixes are not backported"

    Please understand that only bugs that occur in the latest version of
    TopCPToolkit will be addressed. Also, to reduce duplicate efforts,
    fixes cannot be backported to earlier versions.

### Remove customizations

As much as possible, try to isolate the bug to a minimal working example, in one of the analyses currently supported by TopCPToolkit.
If, after removing those additional settings, the bug is gone, the bug was likely caused by your customizations.
A good idea is to add them back gradually to narrow down the root cause of the problem.

### Attempt debugging yourself

You can compile the code in debugging mode to make sure an informative stack trace is printed out, and use `gdb`.
If the bug is related to a common CP algorithm, compiling against a sparse-checkout of Athena can also be helpful.

## I read the above...

Then please click on the button below and use the appropriate template :thumbsup:

[:material-vote: Open an issue](https://gitlab.cern.ch/atlasphys-top/reco/TopCPToolkit/-/issues/new){ .md-button .md-button--primary }