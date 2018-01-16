import liquibook
import random
import time
import uuid
import json

def my_fucking_callback(code, data):
	print(code, json.loads(data.strip()))

liquibook.set_callback(my_fucking_callback)

def generate_order_id():
	return str(uuid.uuid4())

def generate_random_command():
	return "{} {} !IBM {} ;".format(
		['BUY', 'SELL'][random.randint(0,1)],
		random.randint(5,1000),
		random.randint(40,70)
	)
	#if n == 1:
	#	return "CANCEL #{} ;".format()

def random_test():
	start_time = time.time()
	while True:

		myid = generate_order_id()
		mycommand = generate_random_command()
		liquibook.foo(myid, mycommand)
		if time.time() - start_time > 30:
			# liquibook.foo('', 'D + ALL')
			break
		# time.sleep(0.3)

def controlled_test():
	script = """BUY 100 !IBM 49 ;
SELL 10 IBM 40 ;
SELL 10 IBM 40 ;
SELL 80 IBM 40 ;
"""
	for line in script.split('\n'):
		order_id = generate_order_id()
		liquibook.foo(order_id, line.strip())


def modify_test():
	my_id = generate_order_id()
	liquibook.foo(my_id, 'BUY 100 !IBM 49 ;')
	liquibook.foo(my_id, 'MODIFY #{} PRICE 101 QUANTITY -1 ;'.format(my_id))
	liquibook.foo(my_id, 'CANCEL #{} ;'.format(my_id))
		

random_test()
