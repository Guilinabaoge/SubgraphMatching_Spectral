#!/bin/bash

# Options for the user to choose from
options=("Option 1" "Option 2" "Option 3" "Quit")

# Display a menu and prompt the user to choose an option
select choice in "${options[@]}"; do
    case $choice in
        "Option 1")
            echo "You chose Option 1"
            ;;
        "Option 2")
            echo "You chose Option 2"
            ;;
        "Option 3")
            echo "You chose Option 3"
            ;;
        "Quit")
            echo "Goodbye!"
            break
            ;;
        *)
            echo "Invalid option. Please choose a valid option."
            ;;
    esac
done