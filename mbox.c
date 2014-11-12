#include "common.h"
#include "util.h"
#include "mbox.h"
#include "sync.h"


typedef struct
{
  //TODO: Fill this in
  char msg[MAX_MESSAGE_LENGTH];
} Message;

typedef struct
{
  char name[MBOX_NAME_LENGTH];
  //TODO: Fill this in

  int usage_count;
  Message msgs[MAX_MBOX_LENGTH];
  int head;
  int tail;
  semaphore_t empty_count;
  semaphore_t full_count;
  lock_t lock;
  int message_count;
} MessageBox;


static MessageBox MessageBoxen[MAX_MBOXEN];

static void init_single(int i) {

    semaphore_init(&(MessageBoxen[i].empty_count), MAX_MBOX_LENGTH);
    semaphore_init(&(MessageBoxen[i].full_count), 0);
    MessageBoxen[i].head = 0;
    MessageBoxen[i].usage_count = 0;
    MessageBoxen[i].tail = 0;
    MessageBoxen[i].message_count = 0;
}

/* Perform any system-startup
 * initialization for the message
 * boxes.
 */
void init_mbox(void)
{
  (void) MessageBoxen;
  //TODO: Fill this in
  int i;
  for (i = 0; i < MAX_MBOXEN; i++) {
      init_single(i);
      lock_init(&(MessageBoxen[i].lock));
  }
}

/* Opens the mailbox named 'name', or
 * creates a new message box if it
 * doesn't already exist.
 * A message box is a bounded buffer
 * which holds up to MAX_MBOX_LENGTH items.
 * If it fails because the message
 * box table is full, it will return -1.
 * Otherwise, it returns a message box
 * id.
 */
mbox_t do_mbox_open(const char *name)
{
  (void)name;
  //TODO: Fill this in
  int i;
  for (i = 0; i < MAX_MBOXEN; i++) {
    if (same_string(name, MessageBoxen[i].name)) {
        MessageBoxen[i].usage_count++;
        return i;
    }
  }

  for (i = 0; i < MAX_MBOXEN; i++) {
    if (MessageBoxen[i].usage_count == 0) {
        MessageBoxen[i].usage_count++;
        bcopy((char *)name, MessageBoxen[i].name, MAX_MESSAGE_LENGTH);
        return i;
    }
  }

  return -1;
}

/* Closes a message box
 */
void do_mbox_close(mbox_t mbox)
{
  (void)mbox;
  //TODO: Fill this in
  MessageBoxen[mbox].usage_count--;
  if (MessageBoxen[mbox].usage_count <= 0)
      init_single(mbox);
}

/* Determine if the given
 * message box is full.
 * Equivalently, determine
 * if sending to this mbox
 * would cause a process
 * to block.
 */
int do_mbox_is_full(mbox_t mbox)
{
  (void)mbox;
  //TODO: Fill this in
  return MessageBoxen[mbox].message_count == MAX_MBOX_LENGTH;
}

/* Enqueues a message onto
 * a message box.  If the
 * message box is full, the
 * process will block until
 * it can add the item.
 * You may assume that the
 * message box ID has been
 * properly opened before this
 * call.
 * The message is 'nbytes' bytes
 * starting at 'msg'
 * THIS IS THE PRODUCER!!!!!!
 */
void do_mbox_send(mbox_t mbox, void *msg, int nbytes)
{
  (void)mbox;
  (void)msg;
  (void)nbytes;
  //TODO: Fill this in
  semaphore_down(&(MessageBoxen[mbox].empty_count));
  lock_acquire(&(MessageBoxen[mbox].lock));
  bcopy((char *)msg, MessageBoxen[mbox].msgs[MessageBoxen[mbox].tail].msg, nbytes);
  MessageBoxen[mbox].message_count++;
  MessageBoxen[mbox].tail = (MessageBoxen[mbox].tail + 1) % MAX_MBOX_LENGTH;
  lock_release(&(MessageBoxen[mbox].lock));
  semaphore_up(&(MessageBoxen[mbox].full_count));
}

/* Receives a message from the
 * specified message box.  If
 * empty, the process will block
 * until it can remove an item.
 * You may assume that the
 * message box has been properly
 * opened before this call.
 * The message is copied into
 * 'msg'.  No more than
 * 'nbytes' bytes will be copied
 * into this buffer; longer
 * messages will be truncated.
 * THIS IS THE CONSUMER!!!!!!!
 */
void do_mbox_recv(mbox_t mbox, void *msg, int nbytes)
{
  (void)mbox;
  (void)msg;
  (void)nbytes;
  //TODO: Fill this in
  semaphore_down(&(MessageBoxen[mbox].full_count));
  lock_acquire(&(MessageBoxen[mbox].lock));
  bcopy(MessageBoxen[mbox].msgs[MessageBoxen[mbox].head].msg, (char *)msg, nbytes);
  MessageBoxen[mbox].message_count--;
  MessageBoxen[mbox].head = (MessageBoxen[mbox].head + 1) % MAX_MBOX_LENGTH;
  lock_release(&(MessageBoxen[mbox].lock));
  semaphore_up(&(MessageBoxen[mbox].empty_count));
}

/* Returns the number of processes that have
 *  * opened but not closed this mailbox.
 *   */
unsigned int do_mbox_usage_count(mbox_t mbox)
{
  (void)mbox;
  return 0;
  //TODO: Fill this in
  return MessageBoxen[mbox].usage_count;
}

