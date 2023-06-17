import subprocess

for i in range(1, 112):
    with open(f"open_tests/open_{i}.txt", "r") as infile, open("output.txt", "w") as outfile:
        print(i)
        args = "cmake-build-debug/api2023"
        popen = subprocess.Popen(args, stdin=infile, stdout=outfile)
        popen.wait()

        # args = f"git --no-pager diff -- output.txt open_tests/open_{i}.output.txt".split()
        args = f"diff output.txt open_tests/open_{i}.output.txt".split()
        popen = subprocess.Popen(args)
        popen.wait()
