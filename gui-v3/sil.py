def pack(tup) :
    sum = 0
    for i in range(len(tup)) :
        sum |= tup[i]<<(8*i)
    print( sum)

num=(254,254)
pack(num)