import random
import datetime

# --- 配置 ---
NUM_STUDENTS = 1000000
OUTPUT_FILE = "large_data.txt"

# 示例数据
SURNAMES = ["张", "李", "王", "赵", "刘", "陈", "杨", "黄", "吴", "周"]
GIVEN_NAMES = ["三", "四", "五", "六", "七", "八", "九", "十", "明", "伟", "芳", "秀英"]
GENDERS = ["男", "女"]
ADDRESSES = ["北京市", "上海市", "广州市", "深圳市", "杭州市", "南京市", "武汉市", "成都市"]
# ---------

def random_date(start, end):
    """生成一个随机日期"""
    return start + datetime.timedelta(
        seconds=random.randint(0, int((end - start).total_seconds())),
    )

START_DATE = datetime.date(1995, 1, 1)
END_DATE = datetime.date(2005, 12, 31)

print(f"正在生成 {NUM_STUDENTS} 条学生数据到 {OUTPUT_FILE}...")

with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    for i in range(NUM_STUDENTS):
        # 1. 学号 (确保唯一性)
        student_id = f"2025{i:07d}" # e.g., 20250000001
        
        # 2. 姓名
        name = random.choice(SURNAMES) + random.choice(GIVEN_NAMES)
        
        # 3. 出生日期
        birth_date = random_date(START_DATE, END_DATE).strftime("%Y-%m-%d")
        
        # 4. 性别
        gender = random.choice(GENDERS)
        
        # 5. 地址
        address_name = random.choice(ADDRESSES)
        
        # 6. 坐标 X
        coord_x = random.randint(-10000, 10000)
        
        # 7. 坐标 Y
        coord_y = random.randint(-10000, 10000)
        
        line = f"{student_id},{name},{birth_date},{gender},{address_name},{coord_x},{coord_y}\n"
        f.write(line)

print("生成完毕。")