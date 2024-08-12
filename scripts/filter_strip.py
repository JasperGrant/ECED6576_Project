# Python file to correctly format  filter files
# Jasper Grant

fr = open("filters/stage_2.csv", "r")

edited_file = ""

for line in fr.read().split("\n"):
    if line != "":
        edited_file += line + "\n"

fw = open("filters/stage_2.csv", "w")

fw.write(edited_file)
