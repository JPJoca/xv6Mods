In this task, we are creating an application that allows users to count the occurrences of words in text files via the command line. The application is designed to support concurrent scanning of multiple files and updating a hash map structure that tracks word frequencies. User commands allow for adding files for scanning, stopping the application, and searching for word frequencies.

Features:
User Commands:

_count_ <file1>: Adds a new file for scanning. Creates a new scanner thread that scans the text in the file.
_stop_: Shuts down the application.
Word Search: If a user enters a word (or sentence) and presses enter, the last word is used to search for its frequency in the hash map structure.
Implementation Details:
User Interaction:

_count_ Command: Adds a new file to the system and creates a new scanning thread.
_stop_ Command: Shuts down the application.
Word Search: Inputting a word for search displays the current frequency of the word.
Scanner Thread:

Scans the added file and monitors when the file was last modified.
Reads words from the file separated by SPACE, TAB, or ENTER characters.
Ignores words containing characters outside the English alphabet and truncates words longer than 63 characters.
Converts uppercase letters to lowercase and inserts the word into the hash map structure.
After scanning the file, it pauses for five seconds and then checks for file modifications.
Hash Map Structure:

Contains hash keys and values (words and their frequencies).
Implements a hash function that maps strings to numbers.
Handles collisions to avoid data loss.
Allows concurrent insertion and search of values without locking the entire hash map structure.
Concurrency:
N + 1 Threads: N scanner threads and one main thread.
All threads work with the shared hash map structure.
Insertion and search operations in the hash map are performed concurrently, with mutual exclusion for operations on similar values.
Additional Features:
Stop Words: The user program can accept a file containing stop words that should be filtered out during the search. Words that are stop words are not added to the hash map.