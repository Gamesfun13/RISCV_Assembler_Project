
compile:
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly

run:
	@echo "================Running Program...================="
	src/./assembly src/initial.txt src/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff src/ref.txt src/binary.txt
	@echo "---------------------None !------------------------"


risc-assembly: compile run test_rType test_iType test_sType test_bType test_jType test_uType test_pseudo test_Dump1 test_Dump2 test_Dump3 test_Dump4

test_rType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/rType/initial.txt  tests/rType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/rType/binary.txt tests/rType/ref.txt
	@echo "---------------------None !------------------------"

test_iType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/iType/initial.txt  tests/iType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/iType/binary.txt tests/iType/ref.txt
	@echo "---------------------None !------------------------"

test_sType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/sType/initial.txt  tests/sType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/sType/binary.txt tests/sType/ref.txt
	@echo "---------------------None !------------------------"

test_bType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/bType/initial.txt  tests/bType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/bType/binary.txt tests/bType/ref.txt
	@echo "---------------------None !------------------------"
	
test_jType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/jType/initial.txt  tests/jType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/jType/binary.txt tests/jType/ref.txt
	@echo "---------------------None !------------------------"
	
test_uType:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/uType/initial.txt  tests/uType/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/uType/binary.txt tests/uType/ref.txt
	@echo "---------------------None !------------------------"

test_pseudo:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/pseudo/initial.txt  tests/pseudo/binary.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/pseudo/binary.txt tests/pseudo/ref.txt
	@echo "---------------------None !------------------------"
	
	


test_Dump1:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/Project_2/DUMP1_ASSEMBLY.s  tests/Project_2/binary1.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/Project_2/binary1.txt tests/Project_2/DUMP1_MACHINE.out
	@echo "---------------------None !------------------------"
test_Dump2:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/Project_2/DUMP2_ASSEMBLY.s  tests/Project_2/binary2.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/Project_2/binary2.txt tests/Project_2/DUMP2_MACHINE.out
	@echo "---------------------None !------------------------"
test_Dump3:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/Project_2/DUMP3_ASSEMBLY.s  tests/Project_2/binary3.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/Project_2/binary3.txt tests/Project_2/DUMP3_MACHINE.out
	@echo "---------------------None !------------------------"
test_Dump4:
	
	@echo "================Compiling Program...================="
	gcc -g src/skeleton.c -o src/assembly
	@echo "---------------------------------------------------"
	
	@echo "================Running Program...================="
	src/./assembly tests/Project_2/DUMP4_ASSEMBLY.s  tests/Project_2/binary4.txt
	@echo "---------------------------------------------------"
	@echo "================Program Finised...================="
	@echo ""
	@echo "Difference between test output and refernce output"
	@echo "---------------------------------------------------"
	@diff tests/Project_2/binary4.txt tests/Project_2/DUMP4_MACHINE.out
	@echo "---------------------None !------------------------"
	
