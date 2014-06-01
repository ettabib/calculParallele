import java.awt.*;
import java.io.*;
import java.awt.event.*;

public class nsc2ke2 extends Dialog implements MouseListener
{
    String dfname = "DATA"; // to comunicate with program nsc2ke
    int nv, nt;
    Triangle t[];
    Vertex v[];
    int offset = 80, hmax =380 , vmax =180, downoffset = 40 + offset;
    double scale, oldscale, xmin,xmax,ymin,ymax,oldxmin,oldxmax,oldymin
          ,oldymax,zoom0x,zoom1x,zoom0y,zoom1y;
    Button loadtriau, resetzoom, runjob;
    boolean mesh=false;
    TextField textfield0,textfield1, textfield2, textfield3, textfield4, textfield5;
    Label reynolds, mach, angle, cfl, nstep, ftime;
    Checkbox checkbox0, checkbox1, checkbox2,checkbox3;
    CheckboxGroup fluidcheckboxes, timecheckboxes;
    double dreynolds=1000.0, dmach=0.95, dangle=0.1, dcfl=1.0, dtime=2.0;
    int dwhat0=1, dwhat1=1, dnstep=200;
        
public class Vertex { int where; double x,y; void Vertex(){where=0; x=0; y=0;}}
public class Triangle { int where; int[] v; void Triangle(){where=0; }}

public  nsc2ke2(Frame parent) 
{
        super(parent, "nsc2ke", false);
        this.setSize(hmax+2*offset, vmax+4*offset);
    }

public class  getfilename extends Frame
{ 
  FileDialog fd;
  public String fname;
    public getfilename() { }
    public String getthename()
    {   
        fd = new FileDialog(this, "Choose a mesh",FileDialog.LOAD);
        fd.show();
        fname = fd.getFile();
        return fname;
    }
}


public  void readtriau(String fname){
        //int i;
        try{
        FileInputStream filename = new FileInputStream(fname);
        StreamTokenizer file = new StreamTokenizer(filename);
          
      file.parseNumbers();
         
         file.nextToken(); nv = (int)file.nval;
         file.nextToken(); nt= (int)file.nval;
         t = new Triangle[nt];
         v = new Vertex[nv];
         
         for(int i = 0; i<nv;i++) 
         {
            v[i] = new Vertex();
            file.nextToken(); v[i].x = file.nval;
            file.nextToken(); v[i].y = file.nval;
            file.nextToken(); v[i].where = (int)file.nval;
         }
         for(int i = 0; i<nt;i++) 
         {
            t[i] = new Triangle(); t[i].v = new int[3];
            for(int j=0;j<3;j++)
            {
                file.nextToken(); 
                t[i].v[j] = (int)file.nval -1;
            }
            file.nextToken(); t[i].where = (int)file.nval;
         }
         xmin=v[0].x; xmax = v[0].x; ymin=v[0].y; ymax = v[0].y; 
         for(int i = 1; i<nv;i++) 
         {
            if(v[i].x < xmin) xmin = v[i].x;
            if(v[i].x > xmax) xmax = v[i].x;
            if(v[i].y < ymin) ymin = v[i].y;
            if(v[i].y > ymax) ymax = v[i].y;
         }
         
         scale = hmax / (xmax - xmin);  
         if( vmax/ (ymax - ymin) < scale ) scale = vmax/ (ymax - ymin);
         oldscale = scale;
         oldxmin = xmin; oldxmax = xmax;
         oldymin = ymin; oldymax = ymax;
        }
        catch (IOException Ex)
        {
            System.out.println(Ex.getMessage());
        }
    }

public void rline( Graphics g, double x0, double x1, double y0, double y1)
{
                int h0 = offset+(int)(scale * (x0 - xmin));
                int h1 = offset+(int)(scale * (x1 - xmin));
                int v0 = downoffset+(int)(scale * (y0 - ymin));
                int v1 = downoffset+(int)(scale * (y1 - ymin));
                g.drawLine(h0,v0,h1,v1);
}

public void plottriau( Graphics g)
    {            
        g.setColor(Color.blue);
         for(int k = 0; k<nt;k++)
            for(int j = 0; j<3; j++)
            { 
                int i = t[k].v[j];
                int jp = j+1; if(jp==3) jp = 0;
                int ip = t[k].v[jp];
                rline(g, v[i].x, v[ip].x, v[i].y, v[ip].y);
            }
    
    }   

public void myinterface(){
        this.setLayout(new FlowLayout());
        
        loadtriau = new Button("Load Mesh");this.add(loadtriau);
        resetzoom =new Button("Reset Zoom");this.add(resetzoom);
        runjob = new Button("Run Job");  	this.add(runjob);
        
        reynolds = new Label("Re");
        textfield0 = new TextField(Double.toString(dreynolds),5) ;
        this.add(reynolds);  				this.add(textfield0);
        
        mach = new Label("Mach");
        textfield1 = new TextField(Double.toString(dmach),2) ;
        this.add(mach); 					this.add(textfield1);

        angle = new Label("Angle ");
        textfield2 = new TextField(Double.toString(dangle),2) ;
        this.add(angle);  					this.add(textfield2);

        cfl = new Label("CFL");
        textfield3 = new TextField(Double.toString(dcfl),2) ;
        this.add(cfl);  					this.add(textfield3);

        nstep = new Label("nStep");
        textfield4 = new TextField(Integer.toString(dnstep),3) ;
        this.add(nstep);  					this.add(textfield4);
        
        ftime = new Label("Time");
        textfield5 = new TextField(Double.toString(dtime),3) ;
        this.add(ftime);  					this.add(textfield5);

        fluidcheckboxes = new CheckboxGroup();
        checkbox0 = new Checkbox("Euler",fluidcheckboxes,dwhat0==0);
        checkbox1 = new Checkbox("NS",fluidcheckboxes,dwhat0==1);
         this.add(checkbox0);				this.add(checkbox1);
        
        timecheckboxes = new CheckboxGroup();
        checkbox2 = new Checkbox("Unsteady",timecheckboxes,dwhat1==0);
        checkbox3 = new Checkbox("Steady",timecheckboxes,dwhat1==1);
        this.add(checkbox2);  				this.add(checkbox3);        
        this.pack();
        
        Graphics g = this.getGraphics();
    
        loadtriau.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            {  
	            String name ;
	            getfilename ff = new getfilename();
	            name = ff.getthename();
	            readtriau(name);
	            mesh = true;
            	repaint();
            }
        });    
        resetzoom.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            {  
           		xmin = oldxmin;
           	 	xmax = oldxmax;
            	ymin = oldymin;
            	ymax = oldymax;
            	scale = oldscale;
				repaint();
            }
        });    
	    runjob.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e)
            { 
           			savemyparam();
                    Runtime a = Runtime.getRuntime();
                    try{ a.exec("nsc2ke");} 
                        catch (IOException Ex)
                            {   System.out.println(Ex.getMessage()); } 
            		System.exit(0);             
              }
        }); 
        this.addMouseListener(this);
        addMouseListener(this);
}

public void getrunparam()
    {   
        try{
            FileInputStream filename = new FileInputStream(dfname);
            StreamTokenizer file = new StreamTokenizer(filename);
            file.parseNumbers();             
            file.nextToken(); dwhat0 = (int)file.nval;
            file.nextToken(); dreynolds = file.nval;
            file.nextToken(); dmach = file.nval;
            file.nextToken(); dangle = file.nval;
            file.nextToken(); dwhat1 = (int)file.nval;
            file.nextToken(); dcfl = file.nval;
            file.nextToken(); dnstep = (int)file.nval;
            file.nextToken(); dtime = file.nval;
        }
        catch (IOException Ex)
        {
            System.out.println(Ex.getMessage());
        }
    }
    
public void savemyparam() 
    {   String dreynolds = textfield0.getText();
        String dmach = textfield1.getText();
        String dangle = textfield2.getText();
        String dcfl = textfield3.getText();
        String dnstep = textfield4.getText();
        String dtime = textfield5.getText();
        String dwhat0, dwhat1;
        try{
            FileOutputStream filename= new FileOutputStream(dfname);
            PrintStream ffile = new PrintStream(filename);
            if(checkbox0.getState()) ffile.println("0");
                else ffile.println("1");
            ffile.println(dreynolds);
            ffile.println(dmach);
            ffile.println(dangle);
            if(checkbox2.getState()) ffile.println("0");
                else ffile.println("1");
            ffile.println(dcfl);
            ffile.println(dnstep);
            ffile.println(dtime);
        }
        catch (IOException Ex)
        {
            System.out.println(Ex.getMessage());
        }
    }   
    
public void paint( Graphics g ) {    
        if(mesh) plottriau(g);
    }
            
public void mousePressed(MouseEvent e)
    {   int x=e.getX(), y=e.getY();    
        zoom0x = (x-offset)/scale+xmin;
        zoom0y = (y-downoffset)/scale+ymin;
    }   

public void mouseReleased(MouseEvent e)
    {   int x=e.getX(), y=e.getY();
        zoom1x = (x-offset)/scale+xmin;
        zoom1y = (y-downoffset)/scale+ymin;
        double w0 =zoom0x-zoom1x;
        if(w0 < 0)  w0 = - w0;
        if(zoom0y-zoom1y < 0) w0 -= zoom0y-zoom1y; else w0 += zoom0y-zoom1y;
        if(w0 > 1)
        {
            xmin = zoom0x;
            xmax = zoom1x;
            ymin = zoom0y;
            ymax = zoom1y;
            scale = hmax / (xmax - xmin);
        }
        repaint();
    }       
    
public void mouseEntered(MouseEvent e) {;}
public void mouseExited(MouseEvent e)  {;}
public void mouseClicked(MouseEvent e) {;}

public static void main(String[] args){
        Frame f = new Frame("NSC2KE Java Interface");
        nsc2ke2 b = new nsc2ke2(f);
        b.getrunparam();
        b.myinterface();
        b.setSize(640,400);
        b.show();
        
    }
}
