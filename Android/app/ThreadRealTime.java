import static java.lang.Double.parseDouble;

import java.text.DecimalFormat;

public static class ThreadRealTime extends Thread {
    int id;

    public ThreadRealTime() {
        id = MainClass.idss;
        MainClass.idss++;
    }

    @Override
    public void run() {
        try{
            super.run();
            vec[id] = (Number) parseDouble(new DecimalFormat("##.####").format((Number) (MainClass.part * id)));
            MainClass.counter++;
        } catch(InternalError e){
            e.printStackTrace();
        }
    }
}
