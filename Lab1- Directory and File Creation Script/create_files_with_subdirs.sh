#!/bin/bash

# Name of the log file
log_file="script_log.txt" 

# Function to log messages with timestamps
log()
{
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1" >> "$log_file"
}
# Start logging
log "Script started."

# Array of programming languages
languages=("Python" "Java" "C++" "JavaScript" "Ruby" "Rust" "PHP" "C#" "C" "SQL")

# Create main directory with current date and time
main_dir=$(date +'%Y-%m-%d_%H-%M-%S')
mkdir "$main_dir" 

# Log the creation of the main directory
log "Created main directory: $main_dir"

# Create subdirectories called file_101 to file_110
for i in {101..110}; do
    sub_dir="$main_dir/file_$i"
    mkdir "$sub_dir"
    log "Created subdirectory: $sub_dir"

    # Create files in the subdirectory called tuser501.txt to tuser510.txt
    for j in {501..510}; do

        # Determine the programming language assigned to txt based on the file number
        lang_index=$((j - 501))
        language="${languages[$lang_index]}" 

        # Create the file and write the programming language into it
        file_path="$sub_dir/tuser$j.txt"
        echo lanugage: "$language" > "$file_path"
        log "Created file: $file_path with content: lanugage: $language"
    done
done

# End logging
log "Script completed."
# Final message
echo "All files and directories created successfully. Check $log_file for details."