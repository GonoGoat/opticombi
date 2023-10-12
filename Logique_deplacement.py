def Deplacement(posOriginX, posOriginY, posFinishX, posFinishY, tankDir):
    posOrigin = [posOriginX, posOriginY]
    posFinish = [posFinishX, posFinishY]
    left = 0
    right = 0
    up = 0
    down = 0
    disX = posFinish[0] - posOrigin[0]
    disY = posFinish[1] - posOrigin[1]
    Output = ""

    if disX > 0:
        right = disX
        if tankDir != "R" :
            Output += "R"
            tankDir = "R"
        Output += "R" * right
    else :
        left = abs(disX)
        if tankDir != "L" :
            Output += "L"
            tankDir = "L"
        Output += "L" * left
    if disY > 0:
        up = disY
        if tankDir != "U" :
            Output += "U"
            tankDir = "U"
        Output += "U" * up
    else : 
        down = abs(disY)
        if tankDir != "D" :
            Output += "D"
            tankDir = "D"
        Output += "D" * down

    print(Output)

Deplacement(-1, -3, -7, -4, "U")