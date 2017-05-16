



bashCommand = "touch /home/denis/Escritorio/workspaces/Compiladores/main.c" 
import subprocess
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

bashCommand = "lex /home/denis/Escritorio/workspaces/Compiladores/flexer.l"
import subprocess
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()


