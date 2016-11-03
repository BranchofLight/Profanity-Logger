# Profanity-Logger
Profanity Logger was created to capture keys to later be analyzed for their colourful language and not personal information.

All code can be found in this directory. In order to run the following steps are highly recommended:
1. Navigate to /kernmod/
2. Run “make”.
3. Run “sudo ./insert.sh”.
4. Note the major number printed out or “dmesg | cat” for confirmation.
5. Run “sudo ./devload.sh MAJOR” where MAJOR is the major number noted before.
6. Navigate to /userprog/
7. Run “make”.
8. Run “./userproflog”.
