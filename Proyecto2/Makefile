all: clean creator producer consumer finalizer

creator: creator.c
	@gcc -o creator creator.c utilities/semaphore/semaphore.c circ_buff.h circ_buff.c -lrt -lpthread
	@echo creator build done

producer: producer.c
	@gcc -o producer producer.c circ_buff.h circ_buff.c utilities/message/message.c utilities/date/date.c utilities/random/random.c utilities/semaphore/semaphore.c -lrt -lpthread
	@echo producer build done

consumer: consumer.c
	@gcc -o consumer consumer.c circ_buff.h circ_buff.c utilities/message/message.c utilities/semaphore/semaphore.c -lrt -lpthread
	@echo consumer build done

finalizer: finalizer.c
	@gcc -o finalizer finalizer.c utilities/semaphore/semaphore.c -lrt -lpthread
	@echo finalizer build done

clean:
	@rm -f *.o creator producer consumer finalizer
	@echo Clean done
