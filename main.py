from sklearn.cluster import KMeans
arr = []
str = []
k = []
x = []
b = []
centerPoint = []
f = open("sobar-72.csv",'r',encoding = 'utf-8')
arr  = f.readlines()
arr.pop(0)
for i in arr:
    j = i.replace('\n','')
    k = j.split(",")
    for m in k:
        m = float(m)
        str.append(m)
    x.append(str)
    str = []
kmeans = KMeans(n_clusters=3).fit(x)
for i in kmeans.cluster_centers_:
    for y in i:
        t = round(y,3)
        b.append(t)
    centerPoint.append(b)
    b = []
for i in centerPoint:
    print(i)
print(kmeans.inertia_)