import shutil, os

# shutil.copy(".vscode/compile_commands.json", "../../.vscode/compile_commands.json")

if not os.path.exists("../../.vscode/"):
    os.mkdir("../../.vscode/")

if os.path.exists("../../.vscode/compile_commands.json"):
    os.remove("../../.vscode/compile_commands.json")

shutil.copy(".vscode/compile_commands.json", "../../.vscode/compile_commands.json")

