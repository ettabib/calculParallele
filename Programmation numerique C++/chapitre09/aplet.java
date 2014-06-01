// file: aplet.java
import java.applet.*; 
import java.awt.*;

public class aplet extends Applet {

    public void init() {    }

    public void paint( Graphics g ) {
        g.drawString( "Bonjour tous le monde!", 30, 130 );
        g.setColor(Color.pink);
        g.fillOval(10,10,100,100);
    }
}
