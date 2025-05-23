# Movie File Processor Program

My program reads CSV files containing movie data, processes the data into a linked list of movie structures,
and allows the user to interact with the data through a menu.

The user can:
1. Select a file to process based on size (largest, smallest) or specify a file name.
2. Process the selected file to create a directory named "makal.movies.[random_number]".
3. Parse movies by their release year and write each year's movies to separate txt files within the created directory.
4. Exit the program.

Each new directory has 'rwxr-x---' permissions, while the output txt files for each year have 'rw-r-----' permissions.
