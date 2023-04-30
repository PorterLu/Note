import subprocess
import time

# 获取用户输入
program_name = input("请输入程序名：")
n = int(input("请输入执行次数："))

# 执行程序n次，并将执行时间输出到文件
with open("output.txt", "w") as f:
    for i in range(n+1):
        # 第一次执行不计入时间
        if i == 0:
            continue
        
        start_time = time.time()
        subprocess.call([program_name, str(i)])
        end_time = time.time()
        elapsed_time = end_time - start_time
        f.write(f"第{i}次执行时间：{elapsed_time:.2f}秒\n")

