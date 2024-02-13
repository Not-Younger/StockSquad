StockSquad:	Main.o Stock.o Market.o Profile.o
	g++ -Wall Main.o Stock.o Market.o Profile.o /usr/local/cs/cs251/react.o -lcurl

Main.o:	Main.cpp
	g++ -g -Wall -c Main.cpp

Stock.o:	Stock.cpp ../Header\ Files/Stock.h
	g++ -g -Wall -c Stock.cpp

Market.o:	Market.cpp ../Header\ Files/Market.h
	g++ -g -Wall -c Market.cpp

Profile.o:	Profile.cpp ../Header\ Files/Profile.h
	g++ -g -Wall -c Profile.cpp
