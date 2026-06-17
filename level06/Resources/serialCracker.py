import sys

username = sys.argv[1]

hash_value = (ord(username[3]) ^ 4919) + 6221293
for c in username:
	ascii_code = ord(c)

	if ascii_code < 32 or ascii_code > 127:
		print(f"Invalid character {c} in username")
		sys.exit(1)

	hash_value += (ascii_code ^ hash_value) % 1337

print(f"{username} hash is {hash_value}")
