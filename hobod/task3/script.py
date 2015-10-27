import sys

if __name__ == "__main__":
    for request in sys.stdin.readlines():
            request = request.strip()
            slashedParts = request.split("/")
            domain = slashedParts[2].split(".")
            domain[-1]="com"
            slashedParts[2] = ".".join(domain)
            sys.stdout.write(request + "\t" + "/".join(slashedParts) + "\n")