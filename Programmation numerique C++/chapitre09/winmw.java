// file winmw.java
import java.awt.*;
import java.awt.event.*;

public class winmw extends Dialog{
    protected Button bouton1, bouton2, bouton3;
    
    public winmw(Frame parent, String title)
    {
        super(parent, title, false);// creates the window
        this.setLayout(new BorderLayout(100,100));
        bouton1 = new Button("OK");    
        this.add("East",bouton1);  
        bouton2 = new Button("Not OK");    
        this.add("West",bouton2);   
        bouton3 = new Button("Cancel");    
        this.add("South",bouton3);    
        this.pack();
        
        bouton1.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            {   System.out.print("Yes");}
        });    
        bouton2.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            {   System.out.print("No");}
        });
        bouton3.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            {   System.out.print("Cancel");System.exit(0);}
        });
}   
     public static void main(String[] args){
        Frame f = new Frame("win Test");
        f.setSize(400,400);
        Label theline = new Label("From D. Flanagan 1996-99");
        f.add("Center",theline);
        f.show();
        winmw d = new winmw(f,"A window with 2 buttons");
         d.show();
    }
}
