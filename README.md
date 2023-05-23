# Sort analog 

Console sort function.</br> 
it s 50 years old but good for sorting simple text files. </br>

# Description

The sort command sorts the contents of the file in alphabetical or numerological order.</br>
If you specify several files, the sort command will connect them and, having sorted them, will output a single output.</br>


## Build instructions
```sh
# Clone this repo
git clone https://github.com/trpo2023/cw-ip-211_sort.git
cd cw-ip-211_sort
# Run make file
make
# run the program
make run
# if it`s your first time runnig the program type
./sort --help
[aidalinux]:https://aidalinux.ru/w/Sort
```
## For testing (needs googletest)
```sh
# step 1:
cd
# installl google test
sudo apt-get install libgtest-dev
#Step 2:
#install cmake
sudo apt-get install cmake
#Step 3:
cd /usr/src/gtest
sudo cmake CMakeLists.txt
#Step 4:
sudo make
#Step 5:
#You`re done !!
# Then return to repository where project is and run
make test
#if you have an error during step 5 consult this:
https://zwarrior.medium.com/install-google-test-framework-gtest-on-ubuntu-20-04-368eb6951b12
