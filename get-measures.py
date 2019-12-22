import datetime
import requests
import json
import sqlite3


def average(list):
    min_val = min(list)
    max_val = max(list)

    return (sum(list) - min_val - max_val) / (len(list) - 2)


pm_1s = []
pm_2_0s = []
pm_10s = []

for i in range(5):
    r = requests.get('http://pmsensor.local/measurements')
    result = json.loads(r.text)
    if result['ok']:
        pm_1s.append(result['PM1_0'])
        pm_2_0s.append(result['PM2_5'])
        pm_10s.append(result['PM10_0'])

PM1_0 = int(average(pm_1s))
PM2_5 = int(average(pm_2_0s))
PM10_0 = int(average(pm_10s))

print('PM1_0:' + str(PM1_0))
print('PM2_5:' + str(PM2_5))
print('PM10_0:' + str(PM10_0))

connection = sqlite3.connect('measures.db')
connection.execute("insert into measures values ('{}', {}, {}, {})".format(
    datetime.datetime.now(),
    PM1_0,
    PM2_5,
    PM10_0
))
connection.commit()
