# This is a silly little module to remind people to not blindly run
# what we clearly mean as EXAMPLES as the basis for an actual ANALYSIS

import getpass
import os

def warn(arguments):
    # Get the current username
    current_user = getpass.getuser()

    # Define the list of allowed users - the ones who don't get to see the (annoying) warning
    allowed_users = ['ravinab', 'omajersk']

    # Check if the script is running in a CI environment - wouldn't want to annoy the bots either
    is_ci = os.getenv('CI', 'false').lower() == 'true'

    # Do nothing for the VIPs
    if is_ci or current_user in allowed_users:
        return

    # Define the configs that should NOT be used as a starting point
    wrong_yaml   = ['exampleTtbarLjets']
    wrong_python = ['TtbarCPalgoConfigBlocksAnalysis']

    # Do nothing if something else is being run
    if arguments.text_config:
        if not any(config in arguments.text_config for config in wrong_yaml):
            return
    else:
        if not any(analysis in arguments.analysis for analysis in wrong_python):
            return

    # Now the fun starts
    attention_grabber = r'''

    / \   _ __ ___   |_ _|   __| | ___ (_)_ __   __ _   ___  ___  _ __ ___   ___| |_| |__ (_)_ __   __ _   ___(_) | |_   |__ \
   / _ \ | '_ ` _ \   | |   / _` |/ _ \| | '_ \ / _` | / __|/ _ \| '_ ` _ \ / _ \ __| '_ \| | '_ \ / _` | / __| | | | | | |/ /
  / ___ \| | | | | |  | |  | (_| | (_) | | | | | (_| | \__ \ (_) | | | | | |  __/ |_| | | | | | | | (_| | \__ \ | | | |_| |_|
 /_/   \_\_| |_| |_| |___|  \__,_|\___/|_|_| |_|\__, | |___/\___/|_| |_| |_|\___|\__|_| |_|_|_| |_|\__, | |___/_|_|_|\__, (_)
                                                |___/                                              |___/             |___/
'''

    orange_code = "\033[38;5;202m"
    blue_code   = "\033[38;5;39m"
    red_code    = "\033[38;5;196m"
    reset_code  = "\033[0m"

    print("")
    print(f"{red_code}{attention_grabber}{reset_code}")
    print("")
    print("")
    print(f"Hi there. It looks like you are running {orange_code}a configuration that is only meant as an example{reset_code}.")
    print("")
    print("This could go wrong fast, if you are using it as a starting point for your own analysis without being careful!")
    print("")
    print(f"We suggest you head over to {blue_code}https://topcptoolkit.docs.cern.ch/{reset_code}\nand hit the {blue_code}Getting started{reset_code} and {blue_code}Tutorials{reset_code} pages.")
    print("")
    print("If you are just running this config as an example, no worries :) Better safe than sorry.")

    return
