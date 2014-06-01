// file : apletmain.java
import java.awt.*; public class apletmain  extends Frame{
    public apletmain(){
        super("apletmain");
        setSize(200,200);
        show(); // pour afficher la fenetre pour que le pointeur g suivant le soit pas nul
        Graphics g  = getGraphics();
        g.drawString( "Bonjour tous le monde!", 30, 150 );
        g.setColor(Color.pink);
        g.fillOval(40,40,100,100);
        paint(g); // pour peindre 
    }

    public static void main(String[] args){
         apletmain f = new apletmain();
         f.show();  // pour affichier 
     }
}
