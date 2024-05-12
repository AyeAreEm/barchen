import pyperclip

def main():
    with open("gen.txt", "r") as f:
        text = f.read()
        pyperclip.copy(text)

main()
