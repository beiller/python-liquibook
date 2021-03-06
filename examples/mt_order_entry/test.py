import liquibook
import random
import time
import uuid
import json

def my_test_callback(code, data):
	print(code, json.loads(data.strip()))

liquibook.set_callback(my_test_callback)

def generate_order_id():
	return str(uuid.uuid4())

def generate_random_command():
	return "{} {} !IBM {} ;".format(
		['BUY', 'SELL'][random.randint(0,1)],
		random.randint(5,1000),
		[random.randint(40,70), 'MKT'][random.randint(0,1)]
	)
	#if n == 1:
	#	return "CANCEL #{} ;".format()

def random_test():
	start_time = time.time()
	while True:

		myid = generate_order_id()
		mycommand = generate_random_command()
		liquibook.foo(myid, mycommand)

def controlled_test():
	script = """BUY 100.00000001 !XMR/BTC 0.00010001 ;
SELL 100.00000001 !XMR/BTC 0.00010001 ;
"""
	for line in script.split('\n'):
		order_id = generate_order_id()
		liquibook.foo(order_id, line.strip())


def modify_test():
	my_id = generate_order_id()
	liquibook.foo(my_id, 'BUY 100 !IBM 49 ;')
	liquibook.foo(my_id, 'MODIFY #{} PRICE 101 QUANTITY -1 ;'.format(my_id))
	liquibook.foo(my_id, 'CANCEL #{} ;'.format(my_id))
		

if __name__ == "__main__":
	random_test()
