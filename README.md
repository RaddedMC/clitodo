# clitodo
A simple command-line todo list. Made for Linux but will work on Windows with minimal changes.

Usage:
clitodo   -- Reads and pulls up the list
clitodo -a "CONTENTS" "DATE"   -- Adds a new item to the list, will prompt for contents and due date if not given
clitodo -d #index   -- 'Finishes' an item, deleting it from the list
clitodo -m #startdex #enddex   -- Moves an item
clitodo -e #index "CONTENTS" "DATE" -- Changes the contents or due date of an item
clitodo -h   -- Pulls up this help screen

Installation:
Install gcc if you haven't already.
Run sh compile.sh
Copy the output executable to your /bin folder (needs root permission) or run it directly with ./clitodo
Will create a list folder under /home/username/.clitodo-listfile-DONOTEDIT

New to github so bear with me :) Thx for checking out my lil project
