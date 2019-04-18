all: creator producer consumer finalizer

creator: creator.c
	@gcc -o creator creator.c -lrt
	@echo creator build done

producer: producer.c
	@gcc -o producer producer.c utilities/message/message.c utilities/date/date.c utilities/random/random.c -lrt
	@echo producer build done
	
consumer: consumer.c
	@gcc -o consumer consumer.c utilities/message/message.c -lrt
	@echo consumer build done
	
finalizer: finalizer.c
	@gcc -o finalizer finalizer.c -lrt
	@echo finalizer build done

clean:
	@rm -f *.o creator producer consumer finalizer
	@echo Clean done
