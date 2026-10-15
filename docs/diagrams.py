import math, sys, os
from PIL import Image, ImageDraw, ImageFont

OUT = sys.argv[1]
os.makedirs(OUT, exist_ok=True)
S = 2
F = r"C:\Windows\Fonts\arial.ttf"
FB = r"C:\Windows\Fonts\arialbd.ttf"
font = ImageFont.truetype(F, 13 * S)
fsmall = ImageFont.truetype(F, 12 * S)
fbold = ImageFont.truetype(FB, 14 * S)
ftitle = ImageFont.truetype(FB, 17 * S)

def P(x, y): return (x * S, y * S)

def arrowhead(d, tip, frm, hollow=False, size=11):
    ang = math.atan2(tip[1] - frm[1], tip[0] - frm[0])
    a1 = ang + math.radians(150); a2 = ang - math.radians(150)
    p1 = (tip[0] + size * math.cos(a1), tip[1] + size * math.sin(a1))
    p2 = (tip[0] + size * math.cos(a2), tip[1] + size * math.sin(a2))
    pts = [P(*tip), P(*p1), P(*p2)]
    if hollow: d.polygon(pts, fill="white", outline="black", width=S * 1)
    elif hollow is None: d.line([P(*p1), P(*tip), P(*p2)], fill="black", width=S * 1)
    else: d.polygon(pts, fill="black")

def polyline(d, pts, dashed=False, color="black"):
    for (x1, y1), (x2, y2) in zip(pts, pts[1:]):
        if not dashed:
            d.line([P(x1, y1), P(x2, y2)], fill=color, width=int(1.5 * S)); continue
        L = math.hypot(x2 - x1, y2 - y1); n = int(L // 6)
        for i in range(0, n, 2):
            t1, t2 = i / n, min((i + 1) / n, 1)
            d.line([P(x1 + (x2 - x1) * t1, y1 + (y2 - y1) * t1), P(x1 + (x2 - x1) * t2, y1 + (y2 - y1) * t2)], fill=color, width=int(1.5 * S))

def bezier(p0, c, p1, n=40):
    return [((1-t)**2*p0[0]+2*(1-t)*t*c[0]+t*t*p1[0], (1-t)**2*p0[1]+2*(1-t)*t*c[1]+t*t*p1[1]) for t in [i/n for i in range(n+1)]]

def label(d, x, y, text, f=fsmall, bg=True):
    w = d.textlength(text, font=f) / S
    if bg: d.rectangle([P(x - w/2 - 2, y - 8), P(x + w/2 + 2, y + 8)], fill="white")
    d.text(P(x, y), text, font=f, fill="black", anchor="mm")

def arrow(d, pts, text=None, at=None, lab_off=(0, 0)):
    polyline(d, pts); arrowhead(d, pts[-1], pts[-2])
    if text:
        if at is None: at = ((pts[0][0]+pts[1][0])/2, (pts[0][1]+pts[1][1])/2) if len(pts)==2 else pts[len(pts)//2]
        label(d, at[0] + lab_off[0], at[1] + lab_off[1], text)

# ---------------- State Machine Diagram ----------------
W, H = 1300, 960
img = Image.new("RGB", (W * S, H * S), "white"); d = ImageDraw.Draw(img)
label(d, W/2, 22, "State Machine Diagram — «Электронная кофеварка» (класс CoffeeMaker)", ftitle, False)

BW, BH = 176, 62
X = [220, 490, 760, 1030]
Y1, Y2, Y3 = 230, 470, 760
pos = {1: (X[0], Y1), 2: (X[1], Y1), 3: (X[2], Y1), 4: (X[3], Y1),
       5: (X[0], Y2), 6: (X[1], Y2), 7: (X[2], Y2), 8: (X[3], Y2),
       10: (X[0], Y3), 9: (X[3], Y3)}
txt = {1: ["Выкл", "воды нет, кофе нет"], 2: ["Выкл", "вода есть, кофе нет"], 3: ["Выкл", "воды нет, кофе есть"],
       4: ["Выкл", "вода есть, кофе есть"], 5: ["Вкл, Ожидание", "воды нет, кофе нет"], 6: ["Вкл, Ожидание", "вода есть, кофе нет"],
       7: ["Вкл, Ожидание", "воды нет, кофе есть"], 8: ["Вкл, Ожидание", "готова к варке"],
       9: ["Вкл, Варка", "вода есть, кофе есть"], 10: ["Вкл, Подогрев", "кофе готов"]}

# составное состояние «Работа кофеварки»
d.rounded_rectangle([P(28, 58), P(W - 28, 850)], radius=18 * S, outline="black", width=int(1.5 * S))
d.text(P(44, 66), "Работа программы", font=fbold, fill="black")

for k, (x, y) in pos.items():
    fill = "#fde2e2" if k == 1 else ("#e2f0fd" if k in (9, 10) else "#f3f3f3")
    d.rounded_rectangle([P(x - BW/2, y - BH/2), P(x + BW/2, y + BH/2)], radius=14 * S, fill=fill, outline="black", width=int(1.5 * S))
    d.line([P(x - BW/2, y - BH/2 + 22), P(x + BW/2, y - BH/2 + 22)], fill="black", width=S)
    d.text(P(x, y - BH/2 + 11), f"S{k}: {txt[k][0]}", font=fbold, fill="black", anchor="mm")
    d.text(P(x, y + 11), txt[k][1], font=fsmall, fill="black", anchor="mm")

def top(k, dx=0): x, y = pos[k]; return (x + dx, y - BH/2)
def bot(k, dx=0): x, y = pos[k]; return (x + dx, y + BH/2)
def left(k, dy=0): x, y = pos[k]; return (x - BW/2, y + dy)
def right(k, dy=0): x, y = pos[k]; return (x + BW/2, y + dy)

# начальное состояние
ix, iy = X[0] - 50, 110
d.ellipse([P(ix - 9, iy - 9), P(ix + 9, iy + 9)], fill="black")
arrow(d, [(ix, iy + 9), top(1, -50)], "CoffeeMaker()", (ix + 52, iy + 32))

# addWater / addCoffee в верхнем ряду (выкл)
arrow(d, [right(1), left(2)], "addWater()", None, (0, -14))
arrow(d, [right(3), left(4)], "addWater()", None, (0, -14))
for a, b, lab in [(1, 3, "addCoffee()"), (2, 4, "addCoffee()")]:
    pts = bezier(top(a, 40), ((pos[a][0] + pos[b][0]) / 2, Y1 - BH/2 - 95 + (0 if a == 1 else 40)), top(b, -40))
    arrow(d, pts, lab, pts[len(pts)//2])
# в среднем ряду (вкл, ожидание)
arrow(d, [right(5), left(6)], "addWater()", None, (0, -14))
arrow(d, [right(7), left(8)], "addWater()", None, (0, -14))
for a, b, lab, depth in [(5, 7, "addCoffee()", 80), (6, 8, "addCoffee()", 125)]:
    pts = bezier(bot(a, 40), ((pos[a][0] + pos[b][0]) / 2, Y2 + BH/2 + depth), bot(b, -40))
    arrow(d, pts, lab, pts[len(pts)//2])

# change(): выкл <-> вкл
for a, b in [(1, 5), (2, 6), (3, 7), (4, 8)]:
    arrow(d, [bot(a, -14), top(b, -14)])
    arrow(d, [top(b, 14), bot(a, 14)])
    label(d, pos[a][0], (Y1 + Y2) / 2, "change()")

# варка
arrow(d, [bot(8, 0), top(9, 0)], "startBrew()", (X[3], (Y2 + Y3) / 2 + 25), (0, 0))
arrow(d, [left(9), right(10)], "finishBrew()", None, (0, -14))
arrow(d, [top(10, 0), bot(5, 0)], "pourCoffee()", (X[0], 650))
# выключение во время варки и подогрева
xr = W - 46
arrow(d, [right(9), (xr, Y3), (xr, Y1), right(4)], "change() [варка прервана]", (xr - 88, (Y1 + Y3) / 2 + 80))
xl = 46
arrow(d, [left(10), (xl, Y3), (xl, Y1), left(1)], "change()", (xl + 40, (Y1 + Y3) / 2 + 60))

# конечное состояние
fx, fy = W / 2, 915
arrow(d, [(fx, 850), (fx, fy - 13)], "команда 0 «Выход»", (fx + 85, 880))
d.ellipse([P(fx - 13, fy - 13), P(fx + 13, fy + 13)], outline="black", width=int(1.5 * S), fill="white")
d.ellipse([P(fx - 8, fy - 8), P(fx + 8, fy + 8)], fill="black")

img.save(os.path.join(OUT, "state_diagram.png"))

# ---------------- Component Diagram ----------------
W, H = 900, 430
img = Image.new("RGB", (W * S, H * S), "white"); d = ImageDraw.Draw(img)
label(d, W/2, 22, "Диаграмма компонентов (Component Diagram)", ftitle, False)
CW, CH = 190, 56
def comp(x, y, name):
    d.rectangle([P(x - CW/2, y - CH/2), P(x + CW/2, y + CH/2)], fill="#f3f3f3", outline="black", width=int(1.5 * S))
    # значок компонента
    ix, iy = x + CW/2 - 26, y - CH/2 + 8
    d.rectangle([P(ix, iy), P(ix + 16, iy + 20)], outline="black", width=S, fill="white")
    d.rectangle([P(ix - 4, iy + 4), P(ix + 4, iy + 8)], outline="black", width=S, fill="white")
    d.rectangle([P(ix - 4, iy + 12), P(ix + 4, iy + 16)], outline="black", width=S, fill="white")
    d.text(P(x - 10, y), name, font=fbold, fill="black", anchor="mm")
c = {"main": (180, 120, "main.cpp"), "h": (450, 230, "CoffeeMaker.h"), "cpp": (720, 120, "CoffeeMaker.cpp"),
     "tests": (180, 340, "tests.cpp")}
for x, y, n in c.values(): comp(x, y, n)
hx, hy = c["h"][0], c["h"][1]
# main.cpp -> CoffeeMaker.h : использование
pts = [(c["main"][0] + CW/2, c["main"][1]), (hx - 40, hy - CH/2)]
polyline(d, pts, dashed=True); arrowhead(d, pts[1], pts[0], hollow=None, size=16)
label(d, 290, 168, "«use» #include")
# tests.cpp -> CoffeeMaker.h
pts = [(c["tests"][0] + CW/2, c["tests"][1]), (hx - 40, hy + CH/2)]
polyline(d, pts, dashed=True); arrowhead(d, pts[1], pts[0], hollow=None, size=16)
label(d, 290, 292, "«use» #include")
# CoffeeMaker.cpp -> CoffeeMaker.h : реализация
pts = [(c["cpp"][0] - CW/2, c["cpp"][1]), (hx + 40, hy - CH/2)]
polyline(d, pts, dashed=True); arrowhead(d, pts[1], pts[0], hollow=True, size=14)
label(d, 612, 168, "реализация")
# исполняемые файлы
d.text(P(W/2, 395), "coffee.exe = main.cpp + CoffeeMaker.cpp;   tests.exe = tests.cpp + CoffeeMaker.cpp",
       font=fsmall, fill="black", anchor="mm")
img.save(os.path.join(OUT, "component_diagram.png"))
print("ok")


