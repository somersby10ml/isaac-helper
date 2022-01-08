import os

def changeImage(path):
    """
    ex)
        collectibles_001_thesadonion.png -> 1.png
        collectibles_160_ghostbaby.png -> 160.png
    """
    onlyfiles = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]

    rename_fileList = []
    for a in onlyfiles:
        rename_fileList.append({
            'old': a,
            'new': str(int(a.split('_')[1])) + '.png'
        })

    for r in rename_fileList:
        print(r['old'], r['new'])
        os.rename(os.path.join(path, r['old']),
            os.path.join(path, r['new']))


changeImage(R'image\collectibles')
changeImage(R'image\trinkets')
print('finish')