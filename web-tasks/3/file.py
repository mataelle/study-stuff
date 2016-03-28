#1
print("type something")
print(len(input().split(" ")))

#2
import re
print("type file name")
file_name = input()
with open(file_name,"r") as file :
    text = file.read()
    words = re.sub("[\.\,:;\(\)\!\?]", " ", text).split()
    stats = {}
    for word in words :
        if word not in stats.keys() : stats[word] = 1
        else : stats[word] += 1
    print(stats)

#3
def compress (array):
    counts, elems = [1], [array[0]]
    for i in range(1,len(array)):
        if array[i] == elems[-1]:
            counts[-1]+=1
        else:
            counts += [1]
            elems += [array[i]]
    return list(zip(counts, elems))

def decompress (elems):
    lists_of_elems = list(map(lambda a : [a[1]]*a[0], elems))
    return [item for sublist in lists_of_elems for item in sublist]

uncompressed_elems = [1, 2, 2, 2, "test", "test", 2]
compressed_elems = [(1, 1), (3, 2), (2, "test"), (1, 2)]

print(compress(uncompressed_elems))
print(decompress(compressed_elems))

import re
#6.1
textstr = " :-), :), (:, (-:, :D, :-P, ;) :(, :c, :-/, )%"
compiled_re = re.compile("([:;%][-]?[)DP])|([(c][-]?[:;%])")
compiled_re.findall(textstr)

#6.2
with open("usa.txt", "r") as file:
    text = file.read()
    sources = re.findall("\[\d+\]", text)
    print("number of sources:", len(sources), "\n", " ".join(sources))
    years = re.findall("[1-9]{4}", text)
    print("number of years:", len(years), "\n", " ".join(years))
    money = re.findall("\$\d+[.,]?\d*", text)
    print("number of money:", len(money), "\n", " ".join(money))
    percentage = re.findall("\d+[.,]?\d*%", text)
    print("number of money:", len(percentage), "\n", " ".join(percentage))

#6.3
with open("words.txt", "r") as file:
    text = file.read()
    cons = 'qwrtpsdfghjklzxcvbnm'
    vowels = 'euyioaèùìòàéýúíóá'
    print(len(re.findall("["+cons+"]["+vowels+"].*[ \n]", text)))
    print(len(re.findall(".*["+cons+"]{3}.*[ \n]", text)))
    print(len(re.findall(".*["+vowels+"]{3}.*[ \n]", text)))
    print(len(re.findall(".*["+vowels+"]{3}.*[^s][ \n]", text)))

#7
from bs4 import BeautifulSoup
from datetime import datetime, timedelta
from csv import writer

html = open('habr.html', encoding="utf8").read()

colum_names = ["title", "author", "date", "first_line", "hubs"]
rows = []

soup = BeautifulSoup(html)
soup.prettify()

for tag in soup.find_all(class_='post shortcuts_item'):
    title = tag.find('h1').get_text().replace("\n", "")
    author = tag.find('div', 'author').find('a').get_text()

    date = tag.find('div', "published").string.split(" ")
    now_time = datetime.now() - (timedelta(days=1) if len(date[0]) == 5 else 0)
    date = ".".join([str(now_time.day), str(now_time.month), str(now_time.year)])+", "+date[2]

    first_line = tag.find('div', 'content html_format').get_text().split(".")[0]
    while (first_line[0] == " " or first_line[0] == "\n"): first_line = first_line[1:]

    hubs = tag.find_all('a', 'hub')
    hubs = ", ".join([h.get_text() for h in hubs])

    rows.append([title, author, date, first_line, hubs])

csv_file = open("file.csv", "w")
csv_writer = writer(csv_file, delimiter=" ")
csv_writer.writerow(colum_names)
for row in rows : csv_writer.writerow(row)
csv_file.close()

for row in rows :
    for i in range(len(row)-1) : row[i] = '\''+row[i]+'\''
    row[-1] = '[\''+"\', \'".join(row[-1].split(","))+'\']'
for i in range(len(colum_names)) : colum_names[i] = '\''+colum_names[i]+'\''
json_file = open("file.json", "w")
json_file.write("[{")
for row in rows: json_file.write(",".join([colum_names[i]+":"+row[i] for i in range(len(row))])+"},\n{")
json_file.write("}]")
json_file.close()
