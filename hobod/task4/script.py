import argparse
import calendar
import getpass
import happybase
import logging
import random
import sys


USAGE = """
    python script.py 02.10.2014 03.10.2014
""".format(sys.argv[0])


logging.basicConfig(level="INFO")

HOSTS = ["bds%02d.vdi.mipt.ru" % i for i in xrange(7, 10)]
TABLE = "UsersTable"


def connect():
    host = random.choice(HOSTS)
    conn = happybase.Connection(host)

    logging.debug("Connecting to HBase Thrift Server on %s", host)
    conn.open()

    if TABLE not in conn.tables():
        # Create a table with column family `cf` with default settings.
        conn.create_table(TABLE, {"cf": dict()})
        logging.debug("Created table %s", TABLE)
    else:
        logging.debug("Using table %s", TABLE)
    return happybase.Table(TABLE, conn)

def query (datefrom, dateto, table):
    for date, row in table.scan(row_start = datefrom, row_stop = dateto):
        sum = 0
        for page in row.keys():
            sum += int(row[page])
        for page in row.keys():
            print date, "{0:.4f}%".format(100.0*int(row[page])/sum),  page[5:]

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Task4", usage=USAGE)
    parser.add_argument("-datefrom", type=str, required=True)
    parser.add_argument("-dateto", type=str, required=True)

    args = parser.parse_args()
    table = connect()
    query (args.datefrom, args.dateto, table)

