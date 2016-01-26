
all:  
	cd barrier; make 
	cd hello-world; make 
	cd file-writing; make 
	cd parallel-sum; make 
	cd token-ring; make 
	cd monte-carlo; make
	cd monte-carlo.animation; make
	cd parallel-io; make
	cd remote-memory-access; make
	cd spawn-ex1; make
	cd spawn-ex2; make
	cd random; make
	cd bsort; make
	cd prefix-sums; make
	cd mandelbrot; make
	cd alltoall; make
	

zipfile:  clean
	zip --exclude \*.svn\* --exclude \*.settings\* --exclude \*.\*project -r mpi-examples.zip  barrier token-ring file-writing hello-world\
	       parallel-sum monte-carlo monte-carlo.animation parallel-io alltoall \
		   remote-memory-access spawn-ex1 spawn-ex2 random bsort prefix-sums mandelbrot
	cp mpi-examples.zip ../../www/lab/

onyx: clean
	ssh onyx /bin/rm -fr cs430/lab/MPI/*
	scp -r barrier onyx:cs430/lab/MPI/
	scp -r hello-world onyx:cs430/lab/MPI/
	scp -r file-writing onyx:cs430/lab/MPI/
	scp -r parallel-sum onyx:cs430/lab/MPI/
	scp -r token-ring onyx:cs430/lab/MPI/
	scp -r monte-carlo onyx:cs430/lab/MPI/
	scp -r monte-carlo.animation/ onyx:cs430/lab/MPI/
	scp  Makefile onyx:cs430/lab/MPI/
	scp -r parallel-io onyx:cs430/lab/MPI/
	scp -r remote-memory-access onyx:cs430/lab/MPI/
	scp -r spawn-ex1 onyx:cs430/lab/MPI/
	scp -r spawn-ex2 onyx:cs430/lab/MPI/
	scp -r random onyx:cs430/lab/MPI/
	scp -r bsort onyx:cs430/lab/MPI/
	scp -r prefix-sums onyx:cs430/lab/MPI/
	scp -r mandelbrot onyx:cs430/lab/MPI/
	scp -r alltoall onyx:cs430/lab/MPI/

clean:
	cd barrier; make clean 
	cd hello-world; make clean 
	cd file-writing; make clean 
	cd parallel-sum; make clean 
	cd token-ring; make clean 
	cd monte-carlo; make clean
	cd monte-carlo.animation; make clean
	cd parallel-io; make clean
	cd remote-memory-access; make clean
	cd spawn-ex1; make clean
	cd spawn-ex2; make clean
	cd random; make clean
	cd bsort; make clean
	cd prefix-sums; make clean
	cd mandelbrot; make clean
	cd alltoall; make clean
