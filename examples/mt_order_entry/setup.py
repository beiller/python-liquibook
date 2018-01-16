import liquibook
import random
import time
import uuid

def my_fucking_callback(code, data):
	print('callback_code', code, data.strip())

liquibook.set_callback(my_fucking_callback)

script = """BUY 100 !IBM 49 ;
SELL 10 IBM 40 ;
SELL 10 IBM 40 ;
SELL 80 IBM 40 ;
"""

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

def controlled_test():
	for line in script.split('\n'):
		order_id = generate_order_id()
		liquibook.foo(order_id, line.strip())

while True:
	controlled_test()
