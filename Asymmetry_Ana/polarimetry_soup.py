from bs4 import BeautifulSoup
import sys

sys.setrecursionlimit(10000)
f = open("Run15_pol.html")
content = f.read()
#print(content)

outfile_pp = open("polarization_pp.txt", "w")
outfile_pAl = open("polarization_pAl.txt", "w")
outfile_pAu = open("polarization_pAu.txt", "w")

soup = BeautifulSoup(content, 'lxml')
#print(soup.head)
#print(soup.title)
content = soup.body.contents
#print(content)

rows = soup.body.find_all('table')[1].find_all('tr')
print(len(rows))
#print(soup.body.find_all('tr'))

for row in rows:
  columns = row.find_all('td')
  if (len(columns)==0): 
    continue
  #print(columns[0].get_text())  
  #print(columns[5].get_text())
  #print(columns[8].get_text())
  fill = columns[0].get_text()
  bluePolwErr = columns[5].get_text()
  yellowPolwErr = columns[8].get_text()
  
  bluePol = 0
  bluePolErr = 0
  yellowPol = 0
  yellowPolErr = 0
  bluePolList = bluePolwErr.split(" ± ")
  if (len(bluePolList) == 2):
    bluePol = bluePolList[0]
    bluePolErr = bluePolList[1]  
    #print(bluePol)
    #print(bluePolErr)
    
  yellowPolList = yellowPolwErr.split(" ± ")
  if (len(yellowPolList) == 2):
    yellowPol = yellowPolList[0]
    yellowPolErr = yellowPolList[1]  
    #print(yellowPol)
    #print(yellowPolErr)    

  line = [fill.strip(), "  ", str(bluePol).strip(), "  ", str(bluePolErr).strip(), "  ", str(yellowPol).strip(), "  ", str(yellowPolErr).strip() + "\n"]
  line_pA = [fill.strip(), "  ", str(bluePol).strip(), "  ", str(bluePolErr).strip(),"\n"]  
  if (int(fill) <= 18953):
    outfile_pp.writelines(line)
  elif (int(fill) >= 18954 and int(fill) <= 19169):
    outfile_pAu.writelines(line_pA)
  else:
    outfile_pAl.writelines(line_pA)
  
  
  
