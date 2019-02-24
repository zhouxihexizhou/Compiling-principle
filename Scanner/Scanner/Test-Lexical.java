class Dog{

    int &age;

    char _name[];

    float weight_0;

    public Dog(){
        this.&age=3;
        this._name[]="pappy";
        this.weight_0=1.23;
    }

    void do(){
        if(weight_0>1.5){
            weight_0-=0.2;
        }
    }
}