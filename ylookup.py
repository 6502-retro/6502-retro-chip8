print("_rows_lo:")
for i in range(31):
    print("    .lobytes ${:04x}".format(i * 64))
print("_rows_hi:")
for i in range(31):
    print("    .hibytes ${:04x}".format(i * 64))
