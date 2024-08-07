struct buf {
	// blok predstavljen u kodu ima 512 bita
	int flags;
	// cuva da li je disk dirty ili validan

	uint dev;
	uint blockno;
	 // sigurnost da ddva programa istovremeno ne koriste blok
	struct sleeplock lock;

	
	// broj programa koji koriste blok NE ISTOVREMENO
	uint refcnt;
	
	struct buf *prev; // LRU cache list
	struct buf *next;
	// kesiranje za nivo optimizacije po principu LRU
	struct buf *qnext; // disk queue
	uchar data[BSIZE];
};
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x4  // buffer needs to be written to disk

