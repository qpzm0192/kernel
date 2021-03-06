#include "taskDescriptor.h"
#include "bwio.h"


td *schedule(pair *td_pq) {
	//bwputstr(COM2, "schedule\n\r");
	int schedule_i;
	for(schedule_i = 0; schedule_i < 32; schedule_i++) {
		pair p;
		p = td_pq[schedule_i];

		if(p.td_head != 0) {
			td *current_td = p.td_head;
			for(;;) {
				
				if(current_td == 0) break;
				//bwprintf(COM2, "%d, i: %d, id: %d, state: %d\n\r", current_td, schedule_i, current_td->id, current_td->state);

				if(current_td->state == Ready) {
					return current_td;
				}
				else current_td = current_td->td_nxt;
			}
		}
	}
	//no task to be scheduled
	return 0;
}

void pq_insert(pair *td_pq, td *td) {
	//bwprintf(COM2, "insert--id: %d, pri: %d\n\r", td->id, td->priority);
	int priority = td->priority;
	struct taskDescriptor *tail = td_pq[priority].td_tail;
	if(tail != 0) {
		tail->td_nxt = td;
		td->td_prv = tail;
		td->td_nxt = 0;
		td_pq[priority].td_tail = td;
	} else {
		//bwputstr(COM2, "tail == 0\n\r");
		td_pq[priority].td_tail = td;
		td_pq[priority].td_head = td;
		td->td_prv = 0;
		td->td_nxt = 0;
	}
}

void pq_movetoend(pair *td_pq, td *td) {
	//bwprintf(COM2, "movetoend\n\r");
	td->state = Ready;
	int priority = td->priority;

	struct taskDescriptor *tail = td_pq[priority].td_tail;
	struct taskDescriptor *head = td_pq[priority].td_head;

	if(td != tail) {
		//bwputstr(COM2, "move\n\r");
		if(td == head) {
			td_pq[priority].td_head = td->td_nxt;
		} else {
			(td->td_prv)->td_nxt = td->td_nxt;
		}

		td->td_prv = tail;

		tail->td_nxt = td;

		td_pq[priority].td_tail = td;

		td->td_nxt = 0;
	}
}


void pq_delete(pair *td_pq, td *td) {
	td->state = Zombie;
	int priority = td->priority;
	
	struct taskDescriptor *tail = td_pq[priority].td_tail;
	struct taskDescriptor *head = td_pq[priority].td_head;

	if(td == head && td == tail) {
		td_pq[priority].td_head = 0;
		td_pq[priority].td_tail = 0;
	} else if(td == head) {
		td_pq[priority].td_head = td->td_nxt;
		(td->td_nxt)->td_prv = 0;
	} else if(td == tail) {
		td_pq[priority].td_tail = td->td_prv;
		(td->td_prv)->td_nxt = 0;
	} else {
		(td->td_prv)->td_nxt = td->td_nxt;
		(td->td_nxt)->td_prv = td->td_prv;
	}	
}


td* getTaskDes(td* td_ary) {
	int get_i;
	for(get_i = 0; get_i < 88; get_i++) {
		if(td_ary[get_i].free) {
			return &(td_ary[get_i]);
		}
	}
	return 0;
}

