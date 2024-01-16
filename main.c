#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

struct Card
{
    int face;
    int suit;
};

struct Hand
{

    struct Card cards[5];
};



struct Deck
{
    struct Card dealt_cards[52];
    
    int num_Cards_Dealt;
};

struct Hand hand1, hand2;

struct Deck deck;

// to simplify printing

static const char *faceStr[] ={  "Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King","Ace"};
    
static const char *suitStr[] = {  "Hearts","Clubs","Diamonds","Spades"};

static const char *rankStr[] = {"High Card","Single Pair","Two Pair","Three of a Kind","Straight","Flush","Full House","Four of a Kind","Straight Flush"};

int getbet(); //to get the value of bet from user

void generate_Hands(); //it will randomly generate hands

void print_Hands(); //to print hands

struct Card deal_card(); 

void analyze_Hands(); //analyze hands and rank them


void setcolor(int ForgC) //function to change setcolor
 {
 WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                 //Mask out all but the background attribute, and add in the forgournd color
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
 }
 return;
 }
 
int main()
{
	setcolor(10);
printf("************************************************************************************************************************\n");
printf("************************************************************************************************************************\n");
printf("\t\t\t\t\t\tWELCOME TO CASINO: HOM\n\n");
printf("************************************************************************************************************************\n");
printf("************************************************************************************************************************\n");

printf("Here are the rules:\n");
printf("You start with 100 credits, and make a bet from 5 to 50 credits.\n");
printf("You are dealt 5 cards\n");
printf("************************************************************************************************************************\n");
	
	char check;
	int i=0,bet,credit[2];
	credit[0]=100;
	credit[1]=100;
	
	while(1)
	{
		setcolor(25);
	bet=getbet();

    deck.num_Cards_Dealt=0;

    srand(time(0));
    
    generate_Hands();
    setcolor(23);
    print_Hands();

    analyze_Hands(bet,credit);
    
	printf("\nDo you want to continue?\n(Y/N): ");
	scanf(" %c",&check);
	
	if(toupper(check)=='N')
	break;
          }
    return 0;
}

void generate_Hands()
{
    int i;
    
     srand(time(0));
     
    for(i=0; i<5; i++)
	{
        hand1.cards[i] = deal_card();
        hand2.cards[i] = deal_card();
    }

}

struct Card deal_card()
{
    int i;

    int New;

    struct Card deal_card;
    
//    srand(time(0));

    while(1)
	{
        deal_card.face = rand()%13;
        
        deal_card.suit = rand()%4;
        
		New = 1;

        for(i=0; i<deck.num_Cards_Dealt; i++)
		{
			
            if(deck.dealt_cards[i].face == deal_card.face && deck.dealt_cards[i].suit == deal_card.suit)
                
				New = 0;
        }
        
        if(New)
		
		{
            deck.num_Cards_Dealt++;
        
		    deck.dealt_cards[deck.num_Cards_Dealt] = deal_card;
        
		    return deal_card;
        }
    }
}

void print_Hands()
{
    int i;

    printf("\n\nPlayer one got:\n\n");

    for(i=0; i<5; i++)
        
	printf("%s of %s\n", faceStr[hand1.cards[i].face], suitStr[hand1.cards[i].suit]);
    
	printf("\n\nPlayer two got:\n\n");
    
	for(i=0; i<5; i++)
    
	    printf("%s of %s\n", faceStr[hand2.cards[i].face], suitStr[hand2.cards[i].suit]);
}

void analyze_Hands(int bet,int credit[])
{
    int i,j;
    int hand1FaceCounts[13];
    int hand1SuitCounts[4];
    int hand2FaceCounts[13];
    int hand2SuitCounts[4];

    memset(hand1FaceCounts,0,sizeof(hand1FaceCounts));
    memset(hand1SuitCounts,0,sizeof(hand1SuitCounts));
    memset(hand2FaceCounts,0,sizeof(hand2FaceCounts));
    memset(hand2SuitCounts,0,sizeof(hand2SuitCounts));

    for(i=0; i<5; i++)
	{
        hand1FaceCounts[hand1.cards[i].face]++;
        hand1SuitCounts[hand1.cards[i].suit]++;
        hand2FaceCounts[hand2.cards[i].face]++;
        hand2SuitCounts[hand2.cards[i].suit]++;
    }

    // hand1=h1 , hand2=h2
    int h1Rank=0, h1Consec=0, h1Straight=0, h1Flush=0, h1Four=0, h1Three=0, h1Pairs=0;
    int h2Rank=0, h2Consec=0, h2Straight=0, h2Flush=0, h2Four=0, h2Three=0, h2Pairs=0;

    // checking flush (5 cards of the same suit)
    
	for(i=0; i<5; i++)
	{
	    if(hand1SuitCounts[i]==5)
            h1Flush=1;
            
        if(hand2SuitCounts[i]==5)
			h2Flush=1;
	}

	// checking straight for hand1
	i = 0;
	while (hand1FaceCounts[i] == 0)
		i++;
	//     counting consecutive faces
	for (; i < 3 && hand1FaceCounts[i]; i++)
	
		h1Consec++;
	
	if (h1Consec == 5)
	
		h1Straight = 1;

    // checking straight for hand2
	
	i = 0;
	
	while (hand2FaceCounts[i] == 0)
		i++;
	
	// counting consecutive faces 
	
	for (; i < 3 && hand2FaceCounts[i]; i++)
		h2Consec++;
	if (h2Consec == 5)
		h2Straight = 1;


	for (i = 0; i < 14; i++) {
		if (hand1FaceCounts[i] == 4) //checking 4 of a kind
			h1Four = 1;
		if (hand1FaceCounts[i] == 3)  //checking 3 of a kind
			h1Three = 1;
		if (hand1FaceCounts[i] == 2)  //checking pairs
			h1Pairs++;
	}
	for (i = 0; i < 14; i++) {
		if (hand2FaceCounts[i] == 4) //checking 4 of a kind 
			h2Four = 1;
		if (hand2FaceCounts[i] == 3) // checking 3 of a kind
			h2Three = 1;
		if (hand2FaceCounts[i] == 2)  //checking pairs
			h2Pairs++;
	}

	// ranking of hand 1
	
	if (h1Straight && h1Flush)
	
		h1Rank=8;
	
	else if (h1Four)
	
		h1Rank=7;
	
	else if (h1Three && h1Pairs == 1)
	
		h1Rank=6;
	
	else if (h1Flush)
	
		h1Rank=5;
	
	else if (h1Straight)
	
		h1Rank=4;
	
	else if (h1Three)
	
		h1Rank=3;
	
	else if (h1Pairs == 2)
	
		h1Rank=2;
	
	else if (h1Pairs == 1)
	
		h1Rank=1;
	else
	
		h1Rank=0;

    // ranking of hand 2
    
    if (h2Straight && h2Flush)
	
		h2Rank=8;
	
	else if (h2Four)
	
		h2Rank=7;
	
	else if (h2Three && h2Pairs == 1)
	
		h2Rank=6;
	
	else if (h2Flush)
	
		h2Rank=5;
	
	else if (h2Straight)
	
		h2Rank=4;
	
	else if (h2Three)
	
		h2Rank=3;
	
	else if (h2Pairs == 2)
	
		h2Rank=2;
	
	else if (h2Pairs == 1)
	
		h2Rank=1;
	
	else
	
		h2Rank=0;

    // Printing the rank of both hands
    
    printf("\nPlayer one has a %s.\n",rankStr[h1Rank]);
    
	printf("Player two has a %s.\n",rankStr[h2Rank]);

//     Determining winner

    if(h1Rank>h2Rank)
	{
    	credit[0]=credit[0]+bet-bet+bet;
    	
		credit[1]=credit[1]-bet;
		
		if(credit[1]<=0){
			printf("Player Two credit are finished.\n");
			printf("Player One credits: %d\n",credit[0]);
			exit(1);
		}		
        
		printf("Player One Credits: %d\n",credit[0]);
        
		printf("Player Two credits: %d\n",credit[1]);
		
	}

    else if(h2Rank>h1Rank)
	{
    	credit[0]=credit[0]-bet;
    	
		credit[1]=credit[1]+bet+bet-bet;    
		
		if(credit[0]<=0){
			printf("Player One credit are finished.\n");
			printf("Player Two credits: %d\n",credit[1]);
			exit(1);
		}
        
		printf("Player One credits: %d\n",credit[0]);
        
		printf("Player Two credits: %d\n",credit[1]);
		
		}
		
    else
	{
        printf("The two players tie!!!\n");
        
		credit[0]=credit[0]-bet;
        
		credit[1]=credit[1]-bet;
		
		printf("Player One credits are: %d\n",credit[0]);
        
		printf("Player Two credits are: %d\n",credit[1]);
		
		}
		
}



int getbet()

{
	
	
int bet;

do 

{
	
printf("How much do you want to bet? (Enter a number 5 to 50, or 0 to quit the game): ");
scanf(" %d", &bet);

if (bet >= 5 && bet <= 50)

{

return(bet);

}

else if (bet == 0)

{

exit(1);

}

else

{
printf("\n\nPlease enter a bet from 5-50 or 0 to quit the game.\n");


}

}
 while ((bet < 0) || (bet > 5));

}