#include "semaphore.h"

Semaphore::Semaphore(size_t t_max_count) : m_max_count{t_max_count}, m_count{0}
{
	spinlock_init(&m_spinlock);
}

Semaphore::~Semaphore()
{
}

void Semaphore::acquire()
{
	spinlock_acquire(&m_spinlock);
	if (m_count < m_max_count) {
		m_count++;
		spinlock_release(&m_spinlock);
	} else {
		Thread::current->wait_on(m_queue);
		spinlock_release(&m_spinlock);
		acquire();
	}
}

void Semaphore::release()
{
	if (!m_count)
		return;
	spinlock_acquire(&m_spinlock);
	m_queue.wake_up();
	m_count--;
	spinlock_release(&m_spinlock);
}
