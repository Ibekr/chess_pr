#include "iostream"
#include "stdio.h"
#include <vector>
#include <queue>
#include <list>

#include <string>

#define NDEBUG
#include <cassert>


/*
1) Король    K (king)
2) Ферзь     Q (queen)
3) Ладья     R (rook)
4) Конь      N (kNight)
5) Слон      B (bishop)
6) Пешка     P(pawn)


Chess::Desk - вложенный класс, описывает текущую ситуацию на поле

Формат входных данных в файле:
целые числа, разделенные пробелом и переводом строки
Первая строка вида:
P W B
где
P - количество ходов из условия задачи
W - количество белых фигур на доске
B - количество черных фигур на доске
далее N строк вида:
A X Y
где
A - номер фигуры (тип)
X - положение фигуры на доске по координате X (от левого края)
Y - положение фигуры на доске по координате Y (от нижнего ряда)
далее M строк вида:
A X Y

Поле 1 1 (A1) находится в нижнем левом углу доски
Фигуры нумеруются от одного до семи в независимости от цвета
1) Король    K (king)
2) Ферзь     Q (queen)
3) Ладья     R (rook)
4) Конь      N (kNight)
5) Слон      B (bishop)
6) Пешка     P (pawn)
7) Пешка, последний ход которой был двухшаговым
   (Возможно взятие на проходе)
*/


class Chess
{
    public:
        
        Chess();
        
        struct pieceForIn
        {
            int pieceType;
            int xPosition;
            int yPosition;
        };

        struct plyForOut
        {
            int plyNo;
            int pieceType;
            int pieceSymbol;
            int xSourceField;
            int ySourceField;
            int xDestinationField;
            int yDestinationField;
            bool isCastling;
            int whichPieceIfPromotion;
        };

        enum OperatingMode : int
        {
            Generator = 0,//Стандартный режим генерации ходов
            CheckTest = 1,//Режим проверки на шах
            FinalPly = 2 //Завершить генерацию сразу после генерации первого доступного полухода
        };
        
        enum Settings : int
        {
            //задание параметров игрового поля в соответствии с шахматными правилами
            
            DeskSizeX = 8,
            DeskSizeY = 8,
            
            XKingCPosition = 5,
            XLeftRookCPosition = 1,
            XRightRookCPosition = 8,
            YWhiteKingCLine = 1,
            YBlackKingCLine = 8,
            
            YWhiteStartPawnLine = 2, //Линяя, стоя на которой пешка может сделать двухшаговый ход
            YWhitePromotionLine = 7, //Линяя, стоя на которую пешка может сделать ход и сразу превратиться
            YBlackStartPawnLine = 7,
            YBlackPromotionLine = 2,
            
            AmountTypesOfPieces = 6,
            MaxPlys = 50
        };
        
        enum PieceType : int
        {
            //Задание внутренних идентификаторов для разных видов фигур,
            //для пустой клетки и для "бортика", обрамляющего поле.
            
            Empty = 0,
            
            WhiteKing = 1,
            WhiteQueen = 2,
            WhiteRook = 3,
            WhiteKNight = 4,
            WhiteBishop = 5,
            WhitePawn = 6,
            
            BlackIdSum = 100,
            
            BlackKing = WhiteKing + BlackIdSum,
            BlackQueen = WhiteQueen + BlackIdSum,
            BlackRook = WhiteRook + BlackIdSum,
            BlackKNight = WhiteKNight + BlackIdSum,
            BlackBishop = WhiteBishop + BlackIdSum,
            BlackPawn = WhitePawn + BlackIdSum,
            
            DeskBorder = 200
        };
        
        struct ply
        {
            int plyNo;
            int movingPieceType;
            int xSourceField;
            int ySourceField;
            int xDestinationField;
            int yDestinationField;
            int whichPieceIfTaking;
            bool isCastling;
            int whichPieceIfPromotion;
            bool isEnPassantPossiblePrevious;
            int xPosMovedPawnPrevious, yPosMovedPawnPrevious;
            bool isWhiteShortCPermitPrevious, isWhiteLongCPermitPrevious;
            bool isBlackShortCPermitPrevious, isBlackLongCPermitPrevious;
        };
        
        char getPieceSymbol(const int pieceID);
        
        void printDesk(const bool isPreviousPositionShow,
                       const int xPreviousPosition,
                       const int yPreviousPosition);

        void loadChessProblem(const bool isWhiteFirst,
                              const bool isEnPassant,
                              const int xPawnPosIfEnPassant,
                              const int yPawnPosIfEnPassant,
                              const int nMoves,
                              std::list<pieceForIn> * const pieces);
        
        bool computeResolutionRecursion(const int nPlysRest,
                                        const ply *const childPly,
                                        std::queue< std::list<ply> > *const childSolutions);
                                 
        void compute(std::list< std::list<plyForOut> > * const solutionsForOut);
        
        //отладочные функции и данные
        void printTesting();
        bool test();
        void computeTest();
        void loadTestOut(FILE *f);
        std::list<ply> testing;
        std::list<ply> testingOut;
        
    private:
        
        class Desk
        {
            private:
                
                int desk[DeskSizeX + 2][DeskSizeY + 2];
                
                bool isWhiteTurn;
                
                bool isWhiteShortCPermit, isWhiteLongCPermit;
                bool isBlackShortCPermit, isBlackLongCPermit;
                
                bool isEnPassantPossible;
                int xPosMovedPawn, yPosMovedPawn;
                
                inline void moveRookForCastling(const int xKingDestinationField,
                                                const int yKingDestinationField);
                
                int xWhiteKing, yWhiteKing, xBlackKing, yBlackKing;
                
                void initKingPositions();
                void initDesk();
                void setField(const int xPosition, const int yPosition,
                              const int PieceType)
                {
                    desk[xPosition][yPosition] = PieceType;
                }
                
                std::vector<ply> previousPlys;
                
            public:
                Desk();
                
                void switchTurn(){isWhiteTurn = !isWhiteTurn;}
                
                inline bool getIsWhiteTurn(){return isWhiteTurn;}
                
                inline int getField(const int xPosition, const int yPosition)
                {
                    assert(xPosition >= 0 && yPosition >= 0);
                    assert(xPosition <= DeskSizeX + 1 &&
                           yPosition <= DeskSizeY + 1 );
                    
                    return desk[xPosition][yPosition];
                }
                
                //bool setIsWhiteTurn(bool is){isWhiteTurn = is;}
                
                inline bool getIsEnPassantPossible()
                {return isEnPassantPossible;}
                inline int getXPosMovedPawn(){return xPosMovedPawn;}
                inline int getYPosMovedPawn(){return yPosMovedPawn;}
                
                inline int getXWhiteKingPosition(){return xWhiteKing;}
                inline int getYWhiteKingPosition(){return yWhiteKing;}
                inline int getXBlackKingPosition(){return xBlackKing;}
                inline int getYBlackKingPosition(){return yBlackKing;}
                
                inline bool getIsWhiteShortCPermit()
                {return (isWhiteShortCPermit);}
                inline bool getIsWhiteLongCPermit()
                {return (isWhiteLongCPermit);}
                inline bool getIsBlackShortCPermit()
                {return (isBlackShortCPermit);}
                inline bool getIsBlackLongCPermit()
                {return (isBlackLongCPermit);}
                
                void makeMoveAhead(const ply newMove,
                                   const bool isTurnChanging);
                void makeMoveBack(const bool isTurnChanging);
                
                void setDesk(const bool isWhiteFirst,
                             const bool isEnPassant,
                             const int xPawnPosIfEnPassant,
                             const int yPawnPosIfEnPassant,
                             const int totalPlys,
                             std::list<pieceForIn> * const pieces);
            
                bool operator == (const Desk &d1);//для отладки
        };
        
        Desk desk;
        //void printPiece(int pieceID);
        
        int totalPlys;
        
        void addMove(std::queue<ply> *plys,
                     const OperatingMode mode,
                     const int xSourceField,
                     const int ySourceField,
                     const int xDestinationField,
                     const int yDestinationField,
                     const int whichPieceIfTaking,
                     const int isCastling,
                     const int whichPieceIfPromotion);
        
        bool getIsFieldUnderAttack(const int xPosition, const int yPosition);
        
        void generateAllPlys(std::queue<ply> *plys,
                             const OperatingMode mode);
        bool generateKing(std::queue<ply> *plys, const int xPosition,
                          const int yPosition, const OperatingMode mode);
        bool generateQueen(std::queue<ply> *plys, const int xPosition,
                          const int yPosition, const OperatingMode mode);
        bool generateRook(std::queue<ply> *plys, const int xPosition,
                          const int yPosition, const OperatingMode mode);
        bool generateKNight(std::queue<ply> *plys, const int xPosition,
                            const int yPosition, const OperatingMode mode);
        bool generateBishop(std::queue<ply> *plys, const int xPosition,
                            const int yPosition, const OperatingMode mode);
        bool generatePawn(std::queue<ply> *plys, const int xPosition,
                          const int yPosition, const OperatingMode mode);
        bool generateQRB(std::queue<ply> *plys, const bool isMovingLikeRook,
                         const bool isMovingLikeBishop, const int xPosition,
                         const int yPosition, const OperatingMode mode);
        
        inline bool getIsEnemy(const int xPosition, const int yPosition);
        inline bool getIsEmpty(const int xPosition, const int yPosition);
        //bool getIsPiece(int xPosition, int yPosition);
        
        inline bool getIsKing(const int xPosition, const int yPosition);
        inline bool getIsQueen(const int xPosition, const int yPosition);
        inline bool getIsRook(const int xPosition, const int yPosition);
        inline bool getIsKNight(const int xPosition, const int yPosition);
        inline bool getIsBishop(const int xPosition, const int yPosition);
        inline bool getIsPawn(const int xPosition, const int yPosition);
        
};

Chess::Chess()
{
    
}

Chess::Desk::Desk()
{
    
    for(int i = 0; i <= DeskSizeX + 1; i++)
    {
        for(int j = 0; j <= DeskSizeY + 1; j++)
        {
            if((i == 0) || (i == DeskSizeX + 1))
                {desk[i][j] = DeskBorder;}
            else
                {desk[i][j] = Empty;}
        }
        desk[i][0] = DeskBorder;
        desk[i][DeskSizeY + 1] = DeskBorder;
    }
    
    isWhiteTurn = true;
    
    xWhiteKing = 0;
    yWhiteKing = 0;
    xBlackKing = 0;
    yBlackKing = 0;

    isEnPassantPossible = false;
    xPosMovedPawn = 0;
    yPosMovedPawn = 0;
}

void Chess::Desk::initDesk()
{
    
    isWhiteShortCPermit = (desk[XKingCPosition][YWhiteKingCLine] == WhiteKing &&
                                 desk[XRightRookCPosition][YWhiteKingCLine] == WhiteRook);
                                 
    isWhiteLongCPermit = (desk[XKingCPosition][YWhiteKingCLine] == WhiteKing &&
                                 desk[XLeftRookCPosition][YWhiteKingCLine] == WhiteRook);
                                 
    isBlackShortCPermit = (desk[XKingCPosition][YBlackKingCLine] == BlackKing &&
                                  desk[XRightRookCPosition][YBlackKingCLine] == BlackRook);
                                  
    isBlackLongCPermit = (desk[XKingCPosition][YBlackKingCLine] == BlackKing &&
                                 desk[XLeftRookCPosition][YBlackKingCLine] == BlackRook);
    
    initKingPositions();
    
    //track.reserve(MaxPlys);
}


bool Chess::Desk:: operator == (const Desk &d1)
{
    //Оператор сравнения для деск, нужен исключительно для отладки

    for(int i = 0; i <= DeskSizeX + 1; i++)
    {
        for(int j = 0; j <= DeskSizeY + 1; j++)
        {
            if(this->desk[i][j] != d1.desk[i][j])
            return false;
        }
    }
                
    if(this->isWhiteTurn != d1.isWhiteTurn){return false;}
                    
    if(this->isWhiteShortCPermit != d1.isWhiteShortCPermit){return false;}
    if(this->isBlackShortCPermit != d1.isBlackShortCPermit){return false;}
    if(this->isWhiteLongCPermit != d1.isWhiteLongCPermit){return false;}
    if(this->isBlackLongCPermit != d1.isBlackLongCPermit){return false;}
                    
    if(this->isEnPassantPossible != d1.isEnPassantPossible){return false;}
    if(this->xPosMovedPawn != d1.xPosMovedPawn){return false;}
    if(this->yPosMovedPawn != d1.yPosMovedPawn){return false;}
                        
    if(this->xWhiteKing != d1.xWhiteKing){return false;}
    if(this->yWhiteKing != d1.yWhiteKing){return false;}
    if(this->xBlackKing != d1.xBlackKing){return false;}
    if(this->yBlackKing != d1.yBlackKing){return false;}
            
    return true;
}


void Chess::Desk::makeMoveAhead(const ply newMove, const bool isTurnChanging)
{
    
    assert(!(!(desk[newMove.xSourceField][newMove.ySourceField] == BlackKing ||
               desk[newMove.xSourceField][newMove.ySourceField] == WhiteKing) && 
               newMove.isCastling == true));
    assert(!((newMove.yDestinationField != YBlackKingCLine &&
              newMove.yDestinationField != YWhiteKingCLine) &&
              newMove.isCastling == true));
    assert(newMove.whichPieceIfTaking != WhiteKing &&
           newMove.whichPieceIfTaking != BlackKing);
    assert(newMove.movingPieceType == desk[newMove.xSourceField][newMove.ySourceField]);
    assert(newMove.movingPieceType != 0);
    assert(newMove.whichPieceIfPromotion == 0 ||
          (newMove.whichPieceIfPromotion > 0 &&
          (newMove.movingPieceType == WhitePawn ||
           newMove.movingPieceType == BlackPawn)));

    previousPlys.push_back(newMove); 
    
    //Перемещаем фигуру, очищаем исходное поле
    //превращаем пешку, если требуется
    desk[newMove.xDestinationField][newMove.yDestinationField]
      = desk[newMove.xSourceField][newMove.ySourceField];//
    if(newMove.whichPieceIfPromotion > 0)
    {
        desk[newMove.xDestinationField][newMove.yDestinationField] = newMove.whichPieceIfPromotion;//
    }
    else
    {
        desk[newMove.xSourceField][newMove.ySourceField] = Empty;//
    }
    
    if(newMove.movingPieceType == BlackKing ||
       newMove.movingPieceType == WhiteKing)
    {
        if(getIsWhiteTurn())
        {
            xWhiteKing = newMove.xDestinationField;//
            yWhiteKing = newMove.yDestinationField;//
            
            isWhiteShortCPermit = false;//
            isWhiteLongCPermit = false;//
        }
        else
        {
            xBlackKing = newMove.xDestinationField;//
            yBlackKing = newMove.yDestinationField;//
            
            isBlackShortCPermit = false;//
            isBlackLongCPermit = false;//
        }
        
        if(newMove.isCastling)
        {
            moveRookForCastling(newMove.xDestinationField,
                                newMove.yDestinationField);//
        }    
    }

    if(newMove.movingPieceType == BlackRook ||
       newMove.movingPieceType == WhiteRook)
    {
        if(getIsWhiteTurn())
        {
            if(newMove.xSourceField == XRightRookCPosition)
            {
                isWhiteShortCPermit = false;//
            }
            if(newMove.xSourceField == XLeftRookCPosition)
            {
                isWhiteLongCPermit = false;//
            }
        }
        else
        {
            if(newMove.xSourceField == XRightRookCPosition)
            {
                isBlackShortCPermit = false;//
            }
            if(newMove.xSourceField == XLeftRookCPosition)
            {
                isBlackLongCPermit = false;//
            }
        }
    }
    
    if(newMove.movingPieceType == BlackPawn ||
       newMove.movingPieceType == WhitePawn)
    {
        if(isEnPassantPossible)
        {
            int movingPawnId = 0;
            int yTakingPawnPos = 0;
            
            if(getIsWhiteTurn())
            {
                movingPawnId = WhitePawn;
                yTakingPawnPos = newMove.yDestinationField - 1;
            }
            else
            {
                movingPawnId = BlackPawn;
                yTakingPawnPos = newMove.yDestinationField + 1;
            }
            if(xPosMovedPawn == newMove.xDestinationField &&
               yPosMovedPawn == yTakingPawnPos &&
               newMove.movingPieceType == movingPawnId)
            {
                desk[xPosMovedPawn][yPosMovedPawn] = Empty;//
            }
        }
    }


    isEnPassantPossible = false;//
    xPosMovedPawn = 0;//
    yPosMovedPawn = 0;//
    
    if(newMove.movingPieceType == BlackPawn ||
       newMove.movingPieceType == WhitePawn)
    {
        int differencePosY = newMove.yDestinationField - newMove.ySourceField;
        if((newMove.movingPieceType == BlackPawn && differencePosY == -2) ||
           (newMove.movingPieceType == WhitePawn && differencePosY == 2))
        {
            isEnPassantPossible = true;//
            xPosMovedPawn = newMove.xDestinationField;//
            yPosMovedPawn = newMove.yDestinationField;//
        }
    }
    
    if(isTurnChanging){switchTurn();}
    
    assert(desk[xBlackKing][yBlackKing] == BlackKing &&
           desk[xWhiteKing][yWhiteKing] == WhiteKing);

}

void Chess::Desk::makeMoveBack(const bool isTurnChanging)
{
    assert(!previousPlys.empty());
    
    if(isTurnChanging){switchTurn();}
    
    ply newMove = previousPlys.back();
    previousPlys.pop_back();
    
    //Ходим назад и возвращаем взятую фигуру
    desk[newMove.xSourceField][newMove.ySourceField]
      = desk[newMove.xDestinationField][newMove.yDestinationField];//
    desk[newMove.xDestinationField][newMove.yDestinationField]
      = newMove.whichPieceIfTaking;//

    //Превращаем назад в пешку,
    //если было превращение
    if(newMove.whichPieceIfPromotion > 0)
    {
        if(getIsWhiteTurn())
        {
            desk[newMove.xSourceField][newMove.ySourceField]
              = WhitePawn;//
        }
        else
        {
            desk[newMove.xSourceField][newMove.ySourceField]
              = BlackPawn;//
        }
    }


    if(newMove.movingPieceType == BlackKing ||
       newMove.movingPieceType == WhiteKing)
    {
        //printf("MB ");
        if(getIsWhiteTurn())
        {xWhiteKing = newMove.xSourceField; yWhiteKing = newMove.ySourceField;}//
        else
        {xBlackKing = newMove.xSourceField; yBlackKing = newMove.ySourceField;}//
        
        if(newMove.isCastling)
        {
            moveRookForCastling(newMove.xDestinationField,
                                newMove.yDestinationField);//
        }
    }
    
    
    if(newMove.whichPieceIfTaking == BlackPawn ||
       newMove.whichPieceIfTaking == WhitePawn)
    {
        if(newMove.isEnPassantPossiblePrevious)
        {        
                int movingPawnId = 0;
                int yTakingPawnPos = 0;
                
                if(getIsWhiteTurn())
                {
                    movingPawnId = WhitePawn;
                    yTakingPawnPos = newMove.yDestinationField - 1;
                }
                else
                {
                    movingPawnId = BlackPawn;
                    yTakingPawnPos = newMove.yDestinationField + 1;
                }
                
                if(newMove.xPosMovedPawnPrevious == newMove.xDestinationField &&
                   newMove.yPosMovedPawnPrevious == (yTakingPawnPos) &&
                   desk[newMove.xSourceField][newMove.ySourceField] == movingPawnId)
                {
                    desk[newMove.xPosMovedPawnPrevious][newMove.yPosMovedPawnPrevious] = newMove.whichPieceIfTaking;//
                    desk[newMove.xDestinationField][newMove.yDestinationField] = Empty;//
                }
        }
    }
    
    isWhiteShortCPermit = newMove.isWhiteShortCPermitPrevious;
    isWhiteLongCPermit = newMove.isWhiteLongCPermitPrevious;
    isBlackShortCPermit = newMove.isBlackShortCPermitPrevious;
    isBlackLongCPermit = newMove.isBlackLongCPermitPrevious;
    
    isEnPassantPossible = newMove.isEnPassantPossiblePrevious;
    xPosMovedPawn = newMove.xPosMovedPawnPrevious;
    yPosMovedPawn = newMove.yPosMovedPawnPrevious;
    
    assert(desk[xBlackKing][yBlackKing] == BlackKing &&
           desk[xWhiteKing][yWhiteKing] == WhiteKing);

}

inline void Chess::Desk::moveRookForCastling(const int xKingDestinationField,
                                             const int yKingDestinationField)
{
    assert(xKingDestinationField == 3 || xKingDestinationField == 7);
    
    int buff = 0;
    int XRookCPosition = 0;
    int XRookCNewPos = 0;
    
    if(xKingDestinationField == 3)
    {
        XRookCPosition = XLeftRookCPosition;
        XRookCNewPos = XKingCPosition - 1;
    }
    else
    {
        XRookCPosition = XRightRookCPosition;
        XRookCNewPos = XKingCPosition + 1;
    }
    
    buff = desk[XRookCPosition][yKingDestinationField];//
    desk[XRookCPosition][yKingDestinationField] = desk[XRookCNewPos][yKingDestinationField];//
    desk[XRookCNewPos][yKingDestinationField] = buff;//
}

char Chess::getPieceSymbol(const int pieceID)
{
    assert((((pieceID == Empty) || (pieceID >= 1)) && (pieceID <= AmountTypesOfPieces)) ||
           ((pieceID > BlackIdSum) && (pieceID <= BlackIdSum + AmountTypesOfPieces)) || 
           (pieceID == DeskBorder));
    
    switch(pieceID)
    {
        case Empty:{ return '-'; break;}
        
        case WhiteKing:{ return 'K'; break;}
        case WhiteQueen:{ return 'Q'; break;}
        case WhiteRook:{ return 'R'; break;}
        case WhiteKNight:{ return 'N'; break;}
        case WhiteBishop:{ return 'B'; break;}
        case WhitePawn:{ return 'P'; break;}
        
        case BlackKing:{ return 'k'; break;}
        case BlackQueen:{ return 'q'; break;}
        case BlackRook:{ return 'r'; break;}
        case BlackKNight:{ return 'n'; break;}
        case BlackBishop:{ return 'b'; break;}
        case BlackPawn:{ return 'p'; break;}
        
        case DeskBorder:{ return '#'; break;}
    }
    
}

char getXPositionSymbol(int xPosition)
{
    assert(xPosition <= 26);
    
    return char((int)('a') - 1 + xPosition);
}

void Chess::printDesk(const bool isPreviousPositionShow,
                      const int xPreviousPosition, const int yPreviousPosition)
{
    printf(" - 1 2 3 4 5 6 7 8 -\n");
    printf(" - A B C D E F G H -\n");
    for(int j = DeskSizeY + 1; j >= 0; j--)
    {
        for(int i = 0; i <= DeskSizeX + 1; i++)
        {
            if(i == xPreviousPosition && j == yPreviousPosition &&
               isPreviousPositionShow)
            {
                printf(" %c", '*');
            }
            else
            {
                printf(" %c", getPieceSymbol(desk.getField(i,j)));
            }
        }
        if(j > 0 && j < 9) printf("%d\n", j);
        else printf(" \n", j);
    }
    printf(" - A B C D E F G H -\n");
    
    printf("  W%d  W %d_%d B %d_%d ",
    desk.getIsWhiteTurn(),
    desk.getXWhiteKingPosition(),
    desk.getYWhiteKingPosition(),
    desk.getXBlackKingPosition(),
    desk.getYBlackKingPosition());
    
    printf(" EnP: %d %d_%d ",
    desk.getIsEnPassantPossible(),
    desk.getXPosMovedPawn(),
    desk.getYPosMovedPawn());
    
    printf(" WC %d_%d BC %d_%d \n",
    desk.getIsWhiteLongCPermit(),
    desk.getIsWhiteShortCPermit(),
    desk.getIsBlackLongCPermit(),
    desk.getIsBlackShortCPermit());
    printf("\n");
    
}

void Chess::printTesting()
{
    std::list<ply>::iterator i = testing.begin();
     
    if (!testing.empty())
        for (i; i != testing.end(); ++i)
            printf("%d %d %d %d %d\t %d %d\n",
                   i->xSourceField, i->ySourceField,
                   i->xDestinationField, i->yDestinationField,
                   i->whichPieceIfTaking,
                   i->isCastling, i->whichPieceIfPromotion);
}

void Chess::Desk::initKingPositions()
{
    for(int i = 1; i <= DeskSizeX; i++)
    {
        for(int j = 1; j <= DeskSizeY; j++)
        {
            if(desk[i][j] == WhiteKing)
            {
                xWhiteKing = i;
                yWhiteKing = j;
            }
            else
            {
                if(desk[i][j] == BlackKing)
                {
                    xBlackKing = i;
                    yBlackKing = j;
                }
            }
        }
    }
}


bool Chess::getIsFieldUnderAttack(const int xPosition, const int yPosition)
{
    
    if(generateKing(NULL, xPosition, yPosition, CheckTest))
        return true;
    if(generateQueen(NULL, xPosition, yPosition, CheckTest))
        return true;
    if(generateRook(NULL, xPosition, yPosition, CheckTest))
        return true;
    if(generateKNight(NULL, xPosition, yPosition, CheckTest))
        return true;
    if(generateBishop(NULL, xPosition, yPosition, CheckTest))
        return true;
    if(generatePawn(NULL, xPosition, yPosition, CheckTest))
        return true;
    
    return false;
}

void Chess::addMove(std::queue<ply> *plys, const OperatingMode mode,
                    const int xSourceField, const int ySourceField,
                    const int xDestinationField, const int yDestinationField,
                    const int whichPieceIfTaking,
                    const int isCastling, const int whichPieceIfPromotion)
{
    assert(plys != NULL);
    assert(xSourceField <= DeskSizeX &&
           ySourceField <= DeskSizeY &&
           xSourceField > 0 && ySourceField > 0);
    assert(xDestinationField <= DeskSizeX && yDestinationField <= DeskSizeY &&
           xDestinationField > 0 && yDestinationField > 0);
    assert(mode == Generator || mode == FinalPly);
           
    //if(whichPieceIfTaking == WhiteKing || whichPieceIfTaking == BlackKing) 
        //printDesk(false, 0, 0);
            
    assert(!(!(getIsKing(xSourceField, ySourceField)) && 
               isCastling == true));
               
    assert(whichPieceIfTaking != WhiteKing && whichPieceIfTaking != BlackKing); 
    
    
    int xKingPosition;
    int yKingPosition;
    
    ply newMove;
    newMove.plyNo = plys->size();
    newMove.movingPieceType = desk.getField(xSourceField, ySourceField);
    newMove.xSourceField = xSourceField;
    newMove.ySourceField = ySourceField;
    newMove.xDestinationField = xDestinationField;
    newMove.yDestinationField = yDestinationField;
    newMove.whichPieceIfTaking = whichPieceIfTaking;
    newMove.isCastling = isCastling;
    newMove.whichPieceIfPromotion = whichPieceIfPromotion;
    
    newMove.isWhiteShortCPermitPrevious = desk.getIsWhiteShortCPermit();
    newMove.isWhiteLongCPermitPrevious = desk.getIsWhiteLongCPermit();
    newMove.isBlackShortCPermitPrevious = desk.getIsBlackShortCPermit();
    newMove.isBlackLongCPermitPrevious = desk.getIsBlackLongCPermit();
    
    newMove.isEnPassantPossiblePrevious = desk.getIsEnPassantPossible();
    newMove.xPosMovedPawnPrevious = desk.getXPosMovedPawn();
    newMove.yPosMovedPawnPrevious = desk.getYPosMovedPawn();
        
    #ifndef NDEBUG
        //Копируется состояние доски
        //для тестирования функции возврата хода
        Desk* oldDesk = new Desk;
        *oldDesk = desk;
    #endif
    
    desk.makeMoveAhead(newMove, false);
    
    if(desk.getIsWhiteTurn())
    {
        xKingPosition = desk.getXWhiteKingPosition();
        yKingPosition = desk.getYWhiteKingPosition();
    }
    else
    {
        xKingPosition = desk.getXBlackKingPosition();
        yKingPosition = desk.getYBlackKingPosition();
    }
    
    if(getIsFieldUnderAttack(xKingPosition, yKingPosition) == false)
    {
        #ifndef NDEBUG
            //вохранение сгенерированных ходов для проверки
            //правильности прохождения тестов
            if(testing.size() < 300){testing.push_back(newMove);}
        #endif
        
        plys->push(newMove);
        //printDesk(true, xSourceField, ySourceField);
    }
    
    //printDesk(true, xSourceField, ySourceField);
    desk.makeMoveBack(false);
    
    #ifndef NDEBUG
        //Сравнение копии состояния поля до хода
        //с состоянием на доске после возврата хода
        assert(desk == *oldDesk);
        delete oldDesk;
    #endif
    
    //printf("MOVEBACK\n");
}

void Chess::generateAllPlys(std::queue<ply> *plys, const OperatingMode mode)
{
    assert(mode != CheckTest);
    
    int pieceID = 0;
    
    for(int j = DeskSizeY; j >= 1; j--)
    {
        for(int i = 1; i <= DeskSizeX; i++)
        {
            pieceID = desk.getField(i, j);
            if(pieceID > 0)
            {
                if((desk.getIsWhiteTurn() &&
                    (pieceID >= WhiteKing) && (pieceID <= AmountTypesOfPieces)) ||
                   (!desk.getIsWhiteTurn() &&
                    (pieceID >= BlackKing) && (pieceID <= (BlackIdSum + AmountTypesOfPieces))))
                {
                    switch(pieceID)
                    {
                        case WhiteKing :{generateKing(plys, i, j, mode); break;}
                        case WhiteQueen :{generateQueen(plys, i, j, mode); break;}
                        case WhiteRook :{generateRook(plys, i, j, mode); break;}
                        case WhiteKNight :{generateKNight(plys, i, j, mode); break;}
                        case WhiteBishop :{generateBishop(plys, i, j, mode); break;}
                        case WhitePawn :{generatePawn(plys, i, j, mode); break;}
        
                        case BlackKing:{generateKing(plys, i, j, mode); break;}
                        case BlackQueen:{generateQueen(plys, i, j, mode); break;}
                        case BlackRook:{generateRook(plys, i, j, mode); break;}
                        case BlackKNight:{generateKNight(plys, i, j, mode); break;}
                        case BlackBishop:{generateBishop(plys, i, j, mode); break;}
                        case BlackPawn:{generatePawn(plys, i, j, mode); break;}
                    }
                    if(mode == FinalPly)
                    {
                        if(plys->size() > 0)
                        {
                            return;
                        }
                    }
                }
            }
        }
    }
}

void Chess::computeTest()
{
    //std::queue<ply> *plys;
    //plys = new std::queue<ply>;
    std::queue<ply> plys;
    
    #ifndef NDEBUG
        //Копируется состояние доски
        //для тестирования функции возврата хода
        Desk* oldDesk = new Desk;
        *oldDesk = desk;
    #endif
    
    generateAllPlys(&plys, Generator);

    #ifndef NDEBUG
        //Сравнение копии состояния поля до хода
        //с состоянием на доске после возврата хода
        assert(desk == *oldDesk);
        delete oldDesk;
    #endif
    
    //printf("SIZE %d\n", plys.size());
    
    std::queue<ply> empty;
    std::swap( plys, empty );
    //delete plys;
}

bool Chess::computeResolutionRecursion(const int nPlysRest, const ply *const childPly, 
                                       std::queue< std::list<ply> > *const childSolutions)
{
    //bool isRootPly,
    //в функцию передаются childPly и childSolutions,
    //но (по внутренней логики функции) они воспринимаются
    //скорее как parentPly и parentSolutions.
    //Потому, что данная функцция вызывается рекурсивно.
    
    assert(false == ((nPlysRest == 0) && desk.getIsWhiteTurn()));
    assert(nPlysRest >= 0);
    assert(childSolutions != 0);
    //assert(childPly->plyNo >= -1 && childPly->plyNo  < 300);
    //printf("%d ", nPlysRest);
    
    std::queue< std::list<ply> > solutions;
    std::list<ply> path; 
    std::queue<ply> plys;
    
    ply newMove;
    int plyNo = 0;
    //std::list<int> path1;
    
    #ifndef NDEBUG
        //Копируется состояние доски
        //для тестирования функции возврата хода
        Desk* oldDesk = new Desk;
        *oldDesk = desk;
    #endif
    
    if(nPlysRest == 0)
        generateAllPlys(&plys, FinalPly);
    else
        generateAllPlys(&plys, Generator);
    
    #ifndef NDEBUG
        //Сравнение копии состояния поля до хода
        //с состоянием на доске после возврата хода
        assert(desk == *oldDesk);
        delete oldDesk;
    #endif
    
    if(nPlysRest == 0 && plys.size() > 1){assert(0 == 1);}
    assert(plys.size() >= 0);
    

    if(nPlysRest == 0 && plys.size() > 0)
    {
        //последний полуход черных, не мат и не пат.
        return false;
    }
    
    bool isThisPathValid = false;
    bool isReturnedPathsValid = false;
    
    if(plys.size() == 0)
    {
        //Нет возможных ходов
        if(desk.getIsWhiteTurn())
        {
            //ход белых, белым мат или пат
            //printDesk(0, 0, false);
            return false;
        }
        else
        {    
            //Ход черных, черным мат или пат
            if(getIsFieldUnderAttack(desk.getXBlackKingPosition(),
                                     desk.getYBlackKingPosition()))
            {
                //ход черных, черным мат
                #ifndef NDEBUG
                    printf("|");
                #endif
                //printf("\n"); printDesk(0, 0, false);
                return true;
            }
            else
            {
                //ход черных, ходить не могут - пат
                //printf("???");
                //printf("\n"); printDesk(0, 0, false);
                return false;
            }
        }
    }
    
    if(nPlysRest > 0)
    {
        //не мат и не пат
        //ходят черные или белые
        
        while(!plys.empty())
        {
            newMove = plys.front();
            plys.pop();
            
            #ifndef NDEBUG
                Desk* oldDesk = new Desk; *oldDesk = desk;
            #endif
            
            //применение хода и рекурсивный вызов функции, заполняющей список решений
            //(возможную ветвь решений из данного узла дерева решений)
            desk.makeMoveAhead(newMove, true);
            isReturnedPathsValid = computeResolutionRecursion(nPlysRest - 1,
                                                       &newMove, &solutions);
            desk.makeMoveBack(true);
            
            #ifndef NDEBUG
                assert(desk == *oldDesk); delete oldDesk;
            #endif
            //assert(false == (isPath.isReturnedPathsValid == true && isPath.isBlackCut == true));
            
            if(desk.getIsWhiteTurn())
            {
                if(isReturnedPathsValid == true)
                {
                    //гарантированный мат из текущей позиции(то что в desk)
                    //при любых последующих полуходах черных.
                    //возможно данная ветка является частью решения
                    //(если будет сохранена на менее глубоких уровнях рекурсии)
                    //Сохраняем данную ветку (набор путей)
                    
                    if(solutions.empty())
                    {
                        path.clear();
                        if(childPly != NULL){path.push_front(*childPly);}
                        path.push_back(newMove);
                        
                        childSolutions->push(path);
                    }
                    else
                    {    
                        while(!solutions.empty())
                        {
                            path.clear();
                            path.swap(solutions.front());
                            solutions.pop();
                            if(childPly != NULL){path.push_front(*childPly);}
                            childSolutions->push(path);
                        }
                    }
                    
                    
                    isThisPathValid = true;
                    
                    return  true;
                    //while(!solutions.empty())
                    //    solutions.pop();
                }
                
                //(ветвь достоверно не содержит решений)
                //так что вся ветвь просто не сохранена как
                
                assert(solutions.empty());
                
                //while(!solutions.empty())
                //{
                //    solutions.pop();
                //}
            }
            else
            {
                if(isReturnedPathsValid == false)
                {
                    //Данным полуходом черные могут гарантированно
                    //помешать белым поставить мат в условленное число ходов.
                    //Данная ветка достоверно не является частью решения
                    //printf("%d", nPlysRest);
                    return false;
                }
            }
        }
        
        if(!desk.getIsWhiteTurn())
        {
            //Все полуходых черных из данной ситуации на доске
            //являются частью решений,
            //сохраняем их как ветвь дерева решений
            while(!solutions.empty())
            {
                path.clear();
                path.swap(solutions.front());
                solutions.pop();
                if(childPly != NULL){path.push_front(*childPly);}
                childSolutions->push(path);
            }
        }
    }
    
    if(!desk.getIsWhiteTurn()){isThisPathValid = true;}
    return isThisPathValid;
}

bool Chess::generateKing(std::queue<ply> *plys,
                         const int xPosition, const int yPosition,
                         const OperatingMode mode)
{
    //if(isCheckTest){printf("%d %d %d\n", xPosition, yPosition, desk.getField(xPosition, yPosition));}
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    
    assert(getIsKing(xPosition, yPosition) ||
          ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == (XKingCPosition - 1) || xPosition == (XKingCPosition + 1))));
    
    bool isCheckTest = false;
    bool isFastReturning = false;
    if(mode == CheckTest){isCheckTest = true;}
    else
    {
        if(mode == FinalPly){isFastReturning = true;}
    }
    
    int xNewPosition = 0;
    int yNewPosition = 0;
    for(int q = 0; q <= 7; q++)
    {
        switch(q)
        {
            case 0:
            {//ходит вверх
                xNewPosition = xPosition;
                yNewPosition = yPosition + 1;
                break;
            }
            case 1:
            {//ходит вверх-вправо
                xNewPosition = xPosition + 1;
                yNewPosition = yPosition + 1;
                break;
            }
            case 2:
            {//ходит вправо
                xNewPosition = xPosition + 1;
                yNewPosition = yPosition;
                break;
            }
            case 3:
            {//ходит вниз-вправо
                xNewPosition = xPosition + 1;
                yNewPosition = yPosition - 1;
                break;
            }
            case 4:
            {//ходит вниз
                xNewPosition = xPosition;
                yNewPosition = yPosition - 1;
                break;
            }
            case 5:
            {//ходит вниз-влево
                xNewPosition = xPosition - 1;
                yNewPosition = yPosition - 1;
                break;
            }
            case 6:
            {//ходит влево
                xNewPosition = xPosition - 1;
                yNewPosition = yPosition;
                break;
            }
            case 7:
            {//ходит вверх-влево
                xNewPosition = xPosition - 1;
                yNewPosition = yPosition + 1;
                break;
            }
        }
    
        if(desk.getField(xNewPosition, yNewPosition) == 0)
        {
            if(!isCheckTest)
            {
                addMove(plys, mode, xPosition, yPosition, xNewPosition, yNewPosition, 0, false, 0);
                if(isFastReturning && plys->size() > 0){return true;}
                //printf("%d %d\n", xNewPosition, yNewPosition);
            }
        }
        else
        {
            if(getIsEnemy(xNewPosition, yNewPosition))
            {
                if(!isCheckTest)
                {
                    addMove(plys, mode, xPosition, yPosition, xNewPosition, yNewPosition,
                            desk.getField(xNewPosition, yNewPosition), false, 0);
                    if(isFastReturning && plys->size() > 0){return true;}
                    //printf("%d %d\n", xNewPosition, yNewPosition);
                }
                else
                {
                    if(getIsKing(xNewPosition, yNewPosition))
                    {return true;}
                }
            }
        }
    }
    
    if(!isCheckTest)
    {
        int YkingCLine = 0;
        bool isShortCPermit = false;
        bool isLongCPermit = false;
        
        if(desk.getIsWhiteTurn())
        {
            YkingCLine = YWhiteKingCLine;
            isShortCPermit = desk.getIsWhiteShortCPermit();
            isLongCPermit = desk.getIsWhiteLongCPermit();
        }
        else
        {
            YkingCLine = YBlackKingCLine;
            isShortCPermit = desk.getIsBlackShortCPermit();
            isLongCPermit = desk.getIsBlackLongCPermit();
        }
        
        if(isShortCPermit || isLongCPermit)
        {
            if(!getIsFieldUnderAttack(XKingCPosition, YkingCLine))
            {
                if(getIsEmpty((XKingCPosition + 1), YkingCLine) &&
                   getIsEmpty((XKingCPosition + 2), YkingCLine))
                {
                    if(isShortCPermit &&
                       !getIsFieldUnderAttack(XKingCPosition + 1, YkingCLine))
                    {
                        addMove(plys, mode, XKingCPosition, YkingCLine,
                                XKingCPosition + 2, YkingCLine, 0, true, 0);
                        if(isFastReturning && plys->size() > 0){return true;}
                    }
                }
                if(getIsEmpty((XKingCPosition - 1), YkingCLine) &&
                   getIsEmpty((XKingCPosition - 2), YkingCLine) &&
                   getIsEmpty((XKingCPosition - 3), YkingCLine))
                {
                    if(isLongCPermit &&
                       !getIsFieldUnderAttack(XKingCPosition - 1, YkingCLine))
                    {
                        addMove(plys, mode, XKingCPosition, YkingCLine,
                                XKingCPosition - 2, YkingCLine, 0, true, 0);
                        if(isFastReturning && plys->size() > 0){return true;}
                    }
                }
            }
        }
    }
    
    
    return false;
}

bool Chess::generateQueen(std::queue<ply> *plys,
                          const int xPosition, const int yPosition,
                          const OperatingMode mode)
{
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    
    assert(getIsQueen(xPosition, yPosition) ||
           ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
          ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    return generateQRB(plys, 1, 1, xPosition, yPosition, mode);
}

bool Chess::generateRook(std::queue<ply> *plys,
                         const int xPosition, const int yPosition,
                         const OperatingMode mode)
{
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    //printf("!%d %d %d\n", xPosition, yPosition, isCheckTest);
    
    assert(getIsRook(xPosition, yPosition) ||
           ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
          ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    return generateQRB(plys, 1, 0, xPosition, yPosition, mode);
}

//bool Chess::generateKNight(int xPosition, int yPosition, bool isCheck)
bool Chess::generateKNight(std::queue<ply> *plys,
                           const int xPosition, const int yPosition,
                           const OperatingMode mode)
{
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    
    assert(getIsKNight(xPosition, yPosition) ||
          ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
          ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    bool isCheckTest = false;
    bool isFastReturning = false;
    if(mode == CheckTest){isCheckTest = true;}
    else
    {
        if(mode == FinalPly){isFastReturning = true;}
    }
    
    int xNewPosition = 0;
    int yNewPosition = 0;
    for(int q = 0; q <= 7; q++)
    {
        switch(q)
        {
            case 0:
            {//ходит вверх-вверх-вправо
                xNewPosition = xPosition + 1;
                yNewPosition = yPosition + 2;
                break;
            }
            case 1:
            {//ходит вправо-вправо-вверх
                xNewPosition = xPosition + 2;
                yNewPosition = yPosition + 1;
                break;
            }
            case 2:
            {//ходит вправо-вправо-вниз
                xNewPosition = xPosition + 2;
                yNewPosition = yPosition - 1;
                break;
            }
            case 3:
            {//ходит вниз-вниз-вправо
                xNewPosition = xPosition + 1;
                yNewPosition = yPosition - 2;
                break;
            }
            case 4:
            {//ходит вниз-вниз-влево
                xNewPosition = xPosition - 1;
                yNewPosition = yPosition - 2;
                break;
            }
            case 5:
            {//ходит влево-влево-вниз
                xNewPosition = xPosition - 2;
                yNewPosition = yPosition - 1;
                break;
            }
            case 6:
            {//ходит влево-влево-вверх
                xNewPosition = xPosition - 2;
                yNewPosition = yPosition + 1;
                break;
            }
            case 7:
            {//ходит вверх-вверх-влево
                xNewPosition = xPosition - 1;
                yNewPosition = yPosition + 2;
                break;
            }
        }
        //if(xNewPosition >9 || xNewPosition <0 || yNewPosition >9 || yNewPosition <0)
        //printf("KKK %d %d\n",xNewPosition, yNewPosition);
        
        if(xNewPosition <= (DeskSizeX + 1) && 
           yNewPosition <= (DeskSizeY + 1) &&
           xNewPosition >= 0 && yNewPosition >= 0) //Проверка ни перепрыгнет ли конь через "бортик".
        {
            if(desk.getField(xNewPosition, yNewPosition) == 0)
            {
                if(!isCheckTest)
                {
                    addMove(plys, mode, xPosition, yPosition,
                            xNewPosition, yNewPosition, 0, false, 0);
                    if(isFastReturning && plys->size() > 0){return true;}
                    //printf("%d %d\n", xNewPosition, yNewPosition);
                }
            }
            else
            {
                if(getIsEnemy(xNewPosition, yNewPosition))
                {
                    if(!isCheckTest)
                    {
                        addMove(plys, mode, xPosition, yPosition,
                                xNewPosition, yNewPosition,
                                desk.getField(xNewPosition, yNewPosition),
                                false, 0);
                        if(isFastReturning && plys->size() > 0){return true;}
                        //printf("%d %d\n", xNewPosition, yNewPosition);
                    }
                    else
                    {
                        if(getIsKNight(xNewPosition, yNewPosition))
                        {return true;}
                    }
                }
            }
        }
    }
        
    return false;
}

bool Chess::generateBishop(std::queue<ply> *plys,
                           const int xPosition, const int yPosition,
                           const OperatingMode mode)
{
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    
    assert(getIsBishop(xPosition, yPosition) ||
          ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
           ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    return generateQRB(plys, 0, 1, xPosition, yPosition, mode);
}

bool Chess::generateQRB(std::queue<ply> *plys,
                        const bool isMovingLikeRook,
                        const bool isMovingLikeBishop,
                        const int xPosition, const int yPosition,
                        const OperatingMode mode)
{
    assert((xPosition >= 1) && (xPosition <= DeskSizeX));
    assert((yPosition >= 1) && (yPosition <= DeskSizeY));
    assert(isMovingLikeRook || isMovingLikeBishop);
    
    assert(getIsQueen(xPosition, yPosition) ||
           getIsRook(xPosition, yPosition) ||
           getIsBishop(xPosition, yPosition) ||
          ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
           ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    assert(((isMovingLikeRook && isMovingLikeBishop) &&
           getIsQueen(xPosition, yPosition)) ||
           ((isMovingLikeRook && !isMovingLikeBishop) &&
           getIsRook(xPosition, yPosition)) ||
           ((!isMovingLikeRook && isMovingLikeBishop) &&
           getIsBishop(xPosition, yPosition)) ||
          ((mode == CheckTest) && getIsKing(xPosition, yPosition)) ||
           ((mode == CheckTest) &&
          ((desk.getIsWhiteTurn() && yPosition == YWhiteKingCLine) ||
           (!desk.getIsWhiteTurn() && yPosition == YBlackKingCLine)) &&
           (xPosition == 4 || xPosition == 6)));
    
    bool isCheckTest = false;
    bool isFastReturning = false;
    if(mode == CheckTest){isCheckTest = true;}
    else
    {
        if(mode == FinalPly){isFastReturning = true;}
    }
    
    int xNewPosition = 0;
    int yNewPosition = 0;
    int xMovingMultiplier = 0;
    int yMovingMultiplier = 0;
    int iMax = 0;
    int i = 0;
    
    int qStart = 0;
    int qMax = 0;
    
    if(isMovingLikeRook){qStart = 0;}else{qStart = 4;}
    if(isMovingLikeBishop){qMax = 7;}else{qMax = 3;}
    
    for(int q = qStart; q <= qMax; q++)
    {
        xNewPosition = xPosition;
        yNewPosition = yPosition;
        
        switch(q)
        {
            case 0:
            {//ходит вверх
                iMax = DeskSizeY;
                xMovingMultiplier = 0;
                yMovingMultiplier = 1;
                break;
            }
            case 1:
            {//ходит вправо
                iMax = DeskSizeX;
                xMovingMultiplier = 1;
                yMovingMultiplier = 0;
                break;
            }
            case 2:
            {//ходит вниз
                iMax = DeskSizeY;
                xMovingMultiplier = 0;
                yMovingMultiplier = -1;
                break;
            }
            case 3:
            {//ходит влево
                iMax = DeskSizeX;
                xMovingMultiplier = -1;
                yMovingMultiplier = 0;
                break;
            }
            case 4:
            {//ходит вверх-вправо
                iMax = DeskSizeY;
                xMovingMultiplier = 1;
                yMovingMultiplier = 1;
                break;
            }
            case 5:
            {//ходит вниз-вправо
                iMax = DeskSizeY;
                xMovingMultiplier = 1;
                yMovingMultiplier = -1;
                break;
            }
            case 6:
            {//ходит вниз-влево
                iMax = DeskSizeX;
                xMovingMultiplier = -1;
                yMovingMultiplier = -1;
                break;
            }
            case 7:
            {//ходит вверх-влево
                iMax = DeskSizeX;
                xMovingMultiplier = -1;
                yMovingMultiplier = 1;
                break;
            }
        }
        
        for(i = 1; i < iMax; i++)
        {
            xNewPosition = xPosition + (i * xMovingMultiplier);
            yNewPosition = yPosition + (i * yMovingMultiplier);
            if(desk.getField(xNewPosition, yNewPosition) > 0) {break;}
            if(!isCheckTest)
            {
                addMove(plys, mode, xPosition, yPosition,
                        xNewPosition, yNewPosition, 0, false, 0);
                if(isFastReturning && plys->size() > 0){return true;}
                //printf("%d %d\n", xNewPosition, yNewPosition);
            }
        }
        
        if(getIsEnemy(xNewPosition, yNewPosition))
        {
            if(!isCheckTest)
            {
                addMove(plys, mode, xPosition, yPosition,
                        xNewPosition, yNewPosition,
                        desk.getField(xNewPosition, yNewPosition), false, 0);
                if(isFastReturning && plys->size() > 0){return true;}
                //printf("%d %d >\n", xNewPosition, yNewPosition);
            }
            else
            {
                if((isMovingLikeRook && isMovingLikeBishop) &&
                   getIsQueen(xNewPosition, yNewPosition))
                {return true;}
                
                if((isMovingLikeRook && !isMovingLikeBishop) &&
                   getIsRook(xNewPosition, yNewPosition))
                {return true;}
                
                if((!isMovingLikeRook && isMovingLikeBishop) &&
                   getIsBishop(xNewPosition, yNewPosition))
                {return true;}
            }
        }
    }
    
    return false;
}

bool Chess::generatePawn(std::queue<ply> *plys,
                         const int xPosition, const int yPosition,
                         const OperatingMode mode)
{
    bool isCheckTest = false;
    bool isFastReturning = false;
    if(mode == CheckTest){isCheckTest = true;}
    else
    {
        if(mode == FinalPly){isFastReturning = true;}
    }
    
    int yStartLine = 0; //Линяя, стоя на которой пешка может сделать двухшаговый ход
    int yPromotionLine = 0; //Линяя, стоя на которую пешка может сделать ход и сразу превратиться
    int directionMultiplier = 0;
    bool IsPromotionAvailable = false;
        
    int whichPieceIfPromotion = 0;
    int whichPieceIfPromotionTerm = 0;
    int xNewPosition = 0;
    int yNewPosition = 0;
    
    if(desk.getIsWhiteTurn())
    {
        yStartLine = YWhiteStartPawnLine;
        directionMultiplier = 1;
        yPromotionLine = YWhitePromotionLine;
        whichPieceIfPromotionTerm = 0;
    }
    else
    {
        yStartLine = YBlackStartPawnLine;
        directionMultiplier = -1;
        yPromotionLine = YBlackPromotionLine;
        whichPieceIfPromotionTerm = BlackIdSum;
    }
    IsPromotionAvailable = (yPosition == yPromotionLine);

    if(!isCheckTest)
    {
        int qMax = 0;
        if(IsPromotionAvailable)
        {qMax = 4;}
        else
        {qMax = 0;}
        
        for(int q = 0; q <= qMax; q++)
        {
            //при q = 0, генерируется полуходы без превращения пешки
            //при q > 0, генерируются полуходы с превращением пешки в различные фигуры
            
            if(q > 0){whichPieceIfPromotion = q + 1 + whichPieceIfPromotionTerm;}
            else{whichPieceIfPromotion = 0;}
            

            xNewPosition = xPosition;
            yNewPosition = yPosition + directionMultiplier;
            
            if(getIsEmpty(xNewPosition, yNewPosition))
            {
            //одинарный ход вперед пешкой
                addMove(plys, mode, xPosition, yPosition,
                        xNewPosition, yNewPosition,
                        0, false, whichPieceIfPromotion);
                if(isFastReturning && plys->size() > 0){return true;}
                
                if(yStartLine == yPosition)
                {
                    //пешка может сделать двухшаговый ход
                    yNewPosition = yPosition + (2 * directionMultiplier);
                
                    if(getIsEmpty(xNewPosition, yNewPosition))
                    {
                        //двойной ход вперед пешкой
                        addMove(plys, mode, xPosition, yPosition,
                                xNewPosition, yNewPosition,
                                0, false, whichPieceIfPromotion);
                        if(isFastReturning && plys->size() > 0){return true;}
                    }
                }
            }
            for(int i = -1; i <= 1; i = i + 2)
            {
                //Генерируются диаганальные ходы для пешки
                //со взятием фигуры противника
                xNewPosition = xPosition + i;
                yNewPosition = yPosition + directionMultiplier;
                
                if(getIsEnemy(xNewPosition, yNewPosition))
                {
                    //Генерируется полуходы со взятием любой фигуры
                    addMove(plys, mode, xPosition, yPosition,
                            xNewPosition, yNewPosition,
                            desk.getField(xNewPosition, yNewPosition),
                            false, whichPieceIfPromotion);
                    if(isFastReturning && plys->size() > 0){return true;}
                }
                else
                {
                    //Генерируется полуходы со взятием вражеской пешки на проходе
                    //printf("%d %d %d\n",desk.getXPosMovedPawn(),desk.getYPosMovedPawn(),desk.getIsEnPassantPossible());
                    if(desk.getXPosMovedPawn() == xNewPosition &&
                       desk.getYPosMovedPawn() == (yNewPosition - directionMultiplier) &&
                       desk.getIsEnPassantPossible())
                    {
                        addMove(plys, mode, xPosition, yPosition,
                                xNewPosition, yNewPosition,
                                desk.getField(xNewPosition,
                                              yNewPosition - directionMultiplier),
                                false, whichPieceIfPromotion);
                                
                        if(isFastReturning && plys->size() > 0){return true;}
                    }
                }
            }
        }
    }
    else
    {
        for(int i = -1; i <= 1; i = i + 2)
        {
            //проверка, бъёт ли пешка заданное поле (в режиме CheckTest)
            xNewPosition = xPosition + i;
            yNewPosition = yPosition + directionMultiplier;
    
            if(getIsEnemy(xNewPosition, yNewPosition))
            {
                if(getIsPawn(xNewPosition, yNewPosition))
                {return true;}
            }
        }
    }
    return false;
}

inline bool Chess::getIsEnemy(const int xPosition, const int yPosition)
{
    //printf(" %d W%d +", pieceID, isWhiteTurn);
    int pieceID = desk.getField(xPosition, yPosition);
    
    if((desk.getIsWhiteTurn() == true) &&
       ((pieceID >= BlackIdSum + 1) &&
        (pieceID <= BlackIdSum + AmountTypesOfPieces)))
    {return true;}
    
    if((desk.getIsWhiteTurn() == false) &&
       ((pieceID >= 1) && (pieceID <= AmountTypesOfPieces)))
    {return true;}
    
    return false;
}

inline bool Chess::getIsEmpty(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == Empty)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsKing(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhiteKing ||
       desk.getField(xPosition, yPosition) == BlackKing)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsQueen(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhiteQueen ||
       desk.getField(xPosition, yPosition) == BlackQueen)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsRook(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhiteRook ||
       desk.getField(xPosition, yPosition) == BlackRook)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsKNight(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhiteKNight ||
       desk.getField(xPosition, yPosition) == BlackKNight)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsBishop(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhiteBishop ||
       desk.getField(xPosition, yPosition) == BlackBishop)
    {return true;}
    else
    {return false;}
}

inline bool Chess::getIsPawn(const int xPosition, const int yPosition)
{
    if(desk.getField(xPosition, yPosition) == WhitePawn ||
       desk.getField(xPosition, yPosition) == BlackPawn)
    {return true;}
    else
    {return false;}
}

void Chess::Desk::setDesk(const bool isWhiteFirst, const bool isEnPassant,
                          const int xPawnPosIfEnPassant,
                          const int yPawnPosIfEnPassant, const int totalPlys,
                          std::list<pieceForIn> * const pieces)
{
    assert(pieces != NULL);
    assert(!pieces->empty());
    
    isWhiteTurn = isWhiteFirst;
    isEnPassantPossible = isEnPassant;
    if(isEnPassant)
    {
        xPosMovedPawn = xPawnPosIfEnPassant;
        yPosMovedPawn = yPawnPosIfEnPassant;
    }    
    
    std::list<pieceForIn>::iterator i = pieces->begin();
    if (!pieces->empty())
        for (i; i != pieces->end(); ++i)
            setField(i->xPosition, i->yPosition,
                     i->pieceType);
    
    previousPlys.reserve(totalPlys + 2);
    
    initDesk();
}

void Chess::loadChessProblem(const bool isWhiteFirst,
                             const bool isEnPassant,
                             const int xPawnPosIfEnPassant,
                             const int yPawnPosIfEnPassant, const int nMoves,
                             std::list<pieceForIn> * const pieces)
{
    
    totalPlys = nMoves * 2 - 1;
    
    desk.setDesk(isWhiteFirst, isEnPassant,
                 xPawnPosIfEnPassant, yPawnPosIfEnPassant, totalPlys, pieces);
}

void Chess::loadTestOut(FILE *f)
{
    
    ply newMove;
    int movesAmount = 0;
    
    int fErr = 0;
    fErr = fscanf(f, "%d", &movesAmount); 
    assert(fErr == 1);
        
    for(int k = 0; k < movesAmount; k++)
    {
        fErr = fscanf(f, "%d", &newMove.xSourceField);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.ySourceField);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.xDestinationField);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.yDestinationField);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.whichPieceIfTaking);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.isCastling);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &newMove.whichPieceIfPromotion);
        assert(fErr == 1);
        
        testingOut.push_back(newMove);
        
    }
    
}


void Chess::compute(std::list< std::list<plyForOut> > * const solutionsForOut)
{
    //ply emptyRoot;
    //emptyRoot.plyNo = -1;
    
    std::queue< std::list<ply> > solutions;
    this->computeResolutionRecursion(totalPlys, NULL, &solutions);

    printf("\n%d\nPATHS:\n", solutions.size());
    std::list<ply> path;
    
    //std::list< std::list<plyForOut> > solutionsForOut;
    
    std::list<plyForOut> pathForOut;
    plyForOut plyOuter;
    ply plyInner;
    while(!solutions.empty())
    {
        pathForOut.clear();
        
        path = solutions.front();
        solutions.pop();
        
        while(!path.empty())
        {
            plyInner = path.front();
            path.pop_front();
            
            plyOuter.plyNo = plyInner.plyNo;
            plyOuter.pieceType = plyInner.movingPieceType;
            plyOuter.pieceSymbol = Chess::getPieceSymbol(plyInner.movingPieceType);
            plyOuter.xSourceField = plyInner.xSourceField;
            plyOuter.ySourceField = plyInner.ySourceField;
            plyOuter.xDestinationField = plyInner.xDestinationField;
            plyOuter.yDestinationField = plyInner.yDestinationField;
            plyOuter.isCastling = plyInner.isCastling;
            plyOuter.whichPieceIfPromotion = plyInner.whichPieceIfPromotion;
            plyOuter.isCastling = plyInner.isCastling;
            
            pathForOut.push_back(plyOuter);
        }
        
        solutionsForOut->push_back(pathForOut);
        
    }
}

bool Chess::test()
{
    std::list<ply>::iterator i = testing.begin();
     std::list<ply>::iterator j = testingOut.begin();
     
    if(!testingOut.empty() && !testing.empty())
    {
        for(i, j; j != testingOut.end(); ++i, ++j)
        {
            if(i == testing.end()){return false;}
            
            if(i->xSourceField != j->xSourceField ||
               i->ySourceField != j->ySourceField ||
               i->xDestinationField != j->xDestinationField ||
               i->yDestinationField != j->yDestinationField ||
               i->whichPieceIfTaking != j->whichPieceIfTaking ||
               i->whichPieceIfPromotion != j->whichPieceIfPromotion ||
               i->isCastling != j->isCastling)
                {return false;}
        }
        if(i != testing.end()){return false;}
    }
    else{return false;}
        
    return true;
}







bool loadChessProblemFromFile(const std::string fileName,
                              Chess * const chess, const bool isMirror)
{
    FILE *f;
    f = fopen(fileName.c_str(), "r");
    if(f == 0)
    {
        printf("File can't be open or doesn't exist (check the file name)\n");
    }
    assert(f != 0);
    
    //int totalAmount = 0;
    int nMoves;
    int whiteAmount = 0;
    int blackAmount = 0;
    int pId = 0, xPosition = 0, yPosition = 0;
    bool isWhiteFirst = true;
    bool isEnPassantPossible = false;
    int yPawnPosIfEnPassant = 0;
    int xPawnPosIfEnPassant = 0;
    Chess::pieceForIn inputPiece;
    std::list<Chess::pieceForIn> pieces;
    int fErr = 0;
    
    fErr = fscanf(f, "%d", &nMoves);
    assert(fErr == 1);
    //fErr = fscanf(f, "%d", &isWhiteFirst);
    assert(fErr == 1);
    isWhiteFirst = true;
    fErr = fscanf(f, "%d", &whiteAmount);
    assert(fErr == 1);
    fErr = fscanf(f, "%d", &blackAmount);
    assert(fErr == 1);
    //printf("%d ", whiteAmount); 
    //printf("%d\n", blackAmount); 
    nMoves = nMoves;
    
    if(isMirror)
        isWhiteFirst = !isWhiteFirst;
    
    int i = 0;
    int iMax = whiteAmount + blackAmount;
    for(i = 0; i < iMax; i++)
    {
        fErr = fscanf(f, "%d", &pId);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &xPosition);
        assert(fErr == 1);
        fErr = fscanf(f, "%d", &yPosition);
        assert(fErr == 1);
        //printf("%d %d %d\n", p,x,y);
        if(fErr != 1){printf("incorrect input, check input file.\n");}
        
        if(pId == 7)
        {
            //пешка, только что сходившая двухшаговый
            isEnPassantPossible = true;
            xPawnPosIfEnPassant = xPosition;
            yPawnPosIfEnPassant = yPosition;
            pId = 6;
            if(isMirror)
            {
                yPawnPosIfEnPassant = Chess::DeskSizeY - yPosition + 1;
            }
        }
        
        if(i < whiteAmount)
        {
            if(isMirror)
            {
                yPosition = Chess::DeskSizeY - yPosition + 1;
                pId = pId + Chess::BlackIdSum;
            }
        }
        else
        {    
            if(isMirror)
            {
                yPosition = Chess::DeskSizeY - yPosition + 1;
            }
            else
            {
                pId = pId + Chess::BlackIdSum;
            }
        }
        
        inputPiece.pieceType = pId;
        inputPiece.xPosition = xPosition;
        inputPiece.yPosition = yPosition;
        //добавление в список
        pieces.push_back(inputPiece);
    }
    
    fclose(f);
    
    chess->loadChessProblem(isWhiteFirst, isEnPassantPossible,
                            xPawnPosIfEnPassant, yPawnPosIfEnPassant,
                            nMoves, &pieces);
    
    return true;
}

void printResolution(std::list< std::list<Chess::plyForOut> > * const solutions)
{
    assert(solutions != NULL);
    //assert(!solutions->empty());
    
    std::list< std::list<Chess::plyForOut> >::iterator i;
    i = solutions->begin();
     std::list<Chess::plyForOut>::iterator j;
     
    if (!solutions->empty())
        for(i; i != solutions->end(); ++i)
        {
            j = i->begin();
            if(!i->empty())
            for(j; j != i->end(); ++j)
                {
                    printf("  %c%c%d-%c%d", j->pieceSymbol,
                           getXPositionSymbol(j->xSourceField),
                           j->ySourceField,
                           getXPositionSymbol(j->xDestinationField),
                           j->yDestinationField);
                }
            printf("\n");
        }
        printf("\n");
}

void test1()
{
    std::string fileNameIn, fileNameOut, fileNameOutB;
    for(int q = 1; q <= 6; q++)
    {
    
        switch(q)
        {
            case 1:
            {
                fileNameIn = "chess_test_01.txt";
                fileNameOut = "chess_test_01_out.txt";
                fileNameOutB = "chess_test_01_b_out.txt";
                break;
            }
            case 2:
            {
                fileNameIn = "chess_test_02.txt";
                fileNameOut = "chess_test_02_out.txt";
                fileNameOutB = "chess_test_02_b_out.txt";
                break;
            }
            case 3:
            {
                fileNameIn = "chess_test_03.txt";
                fileNameOut = "chess_test_03_out.txt";
                fileNameOutB = "chess_test_03_b_out.txt";
                break;
            }
            case 4:
            {
                fileNameIn = "chess_test_04.txt";
                fileNameOut = "chess_test_04_out.txt";
                fileNameOutB = "chess_test_04_b_out.txt";
                break;
            }
            case 5:
            {
                fileNameIn = "chess_test_05.txt";
                fileNameOut = "chess_test_05_out.txt";
                fileNameOutB = "chess_test_05_b_out.txt";
                break;
            }
            case 6:
            {
                fileNameIn = "chess_test_06.txt";
                fileNameOut = "chess_test_06_out.txt";
                fileNameOutB = "chess_test_06_b_out.txt";
                break;
            }
        }
        
        Chess problem01;
        
        printf(fileNameIn.c_str());
        printf("\n");
        loadChessProblemFromFile(fileNameIn.c_str(), &problem01, false);
        
        //problem01.printDesk(false, 0, 0);
        
        problem01.computeTest();
    
        //problem01.printTesting();
        
        printf(fileNameIn.c_str());
        printf("\n");
        FILE *f;
        f = fopen(fileNameOut.c_str(), "r");
        assert(f != 0);
        problem01.loadTestOut(f);
        fclose(f);
        
        assert(problem01.test());
        
        Chess problem02;
        
        loadChessProblemFromFile(fileNameIn.c_str(), &problem02, false);
        
        printf(fileNameOutB.c_str());
        printf("\n");
        f = fopen(fileNameOutB.c_str(), "r");
        assert(f != 0);
        problem02.loadTestOut(f);
        fclose(f);
        
        assert(problem01.test());
            
    }
}



int main()
{
    #ifndef NDEBUG
        printf("DEBUG MODE\n\n");
    #endif
    
    #ifndef NDEBUG
        test1();
    #endif
    
    printf("WHITE: ");
    printf("K - King; Q - Queen; R - Rook; ");
    printf("N - kNight; B - Bishop; P - Pawn;\n");
    printf("BlACK: ");
    printf("k - King; q - Queen; r - Rook; ");
    printf("n - kNight; b - Bishop; p - Pawn;\n\n");
    printf("Desk:\n");
    
    Chess problem01;
    loadChessProblemFromFile("chess_01.txt", &problem01, false);
    //"chess_01.txt"
    //"chess_02.txt"
    //"chess_03.txt"
    //"chess_04.txt"
    //"chess_05.txt"
    //"chess_06.txt"
    problem01.printDesk(false, 0, 0);
    
    std::list< std::list<Chess::plyForOut> > solutions;
    problem01.compute(&solutions);
    //int a =Chess::DeskSizeX;
    
    printResolution(&solutions);
    
    
    return 0;
}



