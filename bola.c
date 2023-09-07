#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLfloat angle, fAspect;
GLfloat deltaTempo, tempoAtual, tempoAnterior, deltaT;
GLfloat tempoContador = 0;

GLfloat desloc[3] = {0.0f, 0.0f, 0.0f};
GLfloat veloc[3] = {0.0f, 0.0f, 0.0f};
GLfloat forca[3] = {0.0f, 0.0f, 0.0f};

GLfloat grav[3] = {0.0f, -9.81f, 0.0f};   // m/sˆ2
GLfloat massa = 1.0f;

GLfloat slowMotionRatio = 0.1f; // slowMotionRatio Is A Value To Slow Down The Simulation
GLfloat tempoTranscorrido = 0.0f;											
GLfloat velocy0 = 40.0;

void InicializaParticula(void)
{     
     desloc[0] = desloc[1] = desloc[2] = 0.0f;
     veloc[0] = veloc[1] = veloc[2] = 0.0f; 
     forca[0] = forca[1] = forca[2] = 0.0;
}

void CalculaParticula(GLfloat deltaT)
{
     int i;

     // partícula Newtoniana
     for(i=0; i<3; i++) {        
        
        // força da gravidade        
        forca[i] = massa * grav[i];                 
        
        // calcula velocidade
        veloc[i] += (forca[i] / massa) * deltaT;
        
        // calcula novo deslocamento
        desloc[i] += veloc[i] * deltaT;        
     }
        
     if (desloc[1] <= 0.0f) {
     
        InicializaParticula();
	
        //velocy0 /= 1.2;   // coefr = 0.8333
	    veloc[1] = velocy0;
	//printf("veloc=%f\n",veloc[1]);
     }   
        //printf("v=%f, d=%f, f=%f, dt=%f\n",veloc[1],desloc[1],forca[1],deltaT);
}
 
// Função callback chamada para fazer o desenho
void Desenha(void)
{                    
     // Limpa a janela de visualização com a cor de fundo especificada
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // Especifica que a cor corrente é vermelha
     //         R     G     B
     glColor3f(1.0f, 0.0f, 0.0f);
     
     glPushMatrix();
     
        glTranslatef(desloc[0], desloc[1], desloc[2]);
        
        // Desenha uma esfera
        glutSolidSphere(25.0,100,100);
        
     glPopMatrix();
     
     // Executa os comandos OpenGL
     glutSwapBuffers();
}

// Função callback chamada pela GLUT quando não existirem
// eventos a serem processados 
void Idle()
{
    // Guarda tempo atual
    tempoAtual = glutGet(GLUT_ELAPSED_TIME);    
    
    // Calcula tempo transcorrido desde o último frame em segundos
    deltaT = (tempoAtual - tempoAnterior) / 1000.0f;
    
    // Salva tempo atual
    tempoAnterior = tempoAtual;
    
    // Regula slowmotion
    deltaT /= slowMotionRatio;
    
    // Armazena tempo de simulação
    tempoTranscorrido += deltaT;
    
    // Salva tempo atual
    tempoAnterior = tempoAtual;
    
    // Calcula nova posição da partícula
    CalculaParticula(deltaT);	              
    
    // Executa desenho
    glutPostRedisplay();  
}
 
// Inicializa parâmetros de rendering
void Inicializa (void)
{       
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor" 
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
	GLint especMaterial = 80;

 	// Especifica que a cor de fundo da janela será cinza
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);
	
    // Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	
    // Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
    
    // Inicializa sistema de particulas
    InicializaParticula();
    veloc[1] = velocy0;
    
    // Guarda tempo anterior
    tempoAnterior = glutGet(GLUT_ELAPSED_TIME);    
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
   // Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(45.0f,fAspect,0.1,500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0,0,200, 0,0,0, 0,1,0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
    
	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  
		  slowMotionRatio += 0.1;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  
		  slowMotionRatio -= 0.1;            
		}
		
	glutPostRedisplay();
}

// Programa Principal 
int main(int argc, char **argv)
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
     glutInitWindowSize(800,600);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Particula");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     glutIdleFunc(Idle);
     glutMouseFunc(GerenciaMouse);
     Inicializa();
     glutMainLoop();
}
