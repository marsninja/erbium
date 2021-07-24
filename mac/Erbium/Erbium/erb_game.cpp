/*---------------------------------------------------------------------------
Erbium  Game Engine

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_game.h"

namespace erb { namespace game {
//Erbium Assembler-----------------------------------------------------------
game_object::game_object():auto_color(rand()){}

void game_object::draw()
{
  srand(auto_color);
  glPushMatrix();
  for(u_long i=0; i<br.size(); i++)
    for(u_long j=0; j<br[i].poly.size(); j++)
    {
      glBegin(GL_POLYGON);
      for(u_long k=br[i][j].vert.size(); k>0; k--)
      {
        glColor3f((float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX);
        glVertex3f(br[i][j][k-1].x,br[i][j][k-1].y,br[i][j][k-1].z);     
      }
      glEnd();
    }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPopMatrix(); 
}
    
void draw(construct &dc)
    {
        srand(50);
        glPushMatrix();
        for(u_long i=0; i<dc.br.size(); i++)
            for(u_long j=0; j<dc.br[i].poly.size(); j++)
            {
                glBegin(GL_POLYGON);
                for(u_long k=dc.br[i][j].vert.size(); k>0; k--)
                {
                    glColor3f((float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX);
                    glVertex3f(dc.br[i][j][k-1].x,dc.br[i][j][k-1].y,dc.br[i][j][k-1].z);
                }
                glEnd();
            }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPopMatrix();
    }
//------------------------------------------------------------------------END
}}
