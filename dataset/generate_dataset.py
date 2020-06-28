from crate import client
import csv

connection = client.connect('http://localhost:4200')
cursor = connection.cursor()
cursor.execute("SELECT * FROM mtopeniot.etsensor WHERE entity_id = ? ORDER BY time_index ASC", ("Sensor:001",))
rows = cursor.fetchall()

f = csv.writer(open("dataset.csv", 'w'))
first_row = [column[0] for column in cursor.description]
f.writerow(first_row)

for row in rows:
    f.writerow(row)
