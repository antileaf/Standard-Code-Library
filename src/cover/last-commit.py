import datetime, subprocess

result = subprocess.run(["git", "log", "-n", "1"], stdout = subprocess.PIPE)

for bytes in result.stdout.splitlines():
	line = bytes.decode()

	if line.startswith("commit"):
		commit = line.split()[1]
	elif line.startswith("Merge:"):
		merge = (line.split()[1], line.split()[2])
	elif line.startswith("Author:"):
		author = line.split()[1] # No need for email
	elif line.startswith("Date:"):
		date_str = " ".join(line.split()[1:-1]) # No need for timezone
		date_format = "%a %b %d %H:%M:%S %Y"
		date = datetime.datetime.strptime(date_str, date_format)
	elif line.startswith("    "):
		message = line.strip()

print(f"{date.strftime('%h %d, %Y')} ({message})"
	.replace("&", "\\&")
	.replace("%", "\\%")
	.replace("#", "\\#")) # LaTeX escape
