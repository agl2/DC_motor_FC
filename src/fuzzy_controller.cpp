#include "../systemc-2.3.1/src/systemc.h"
#include "../headers/fuzzy_controller.h"

void fuzzy_controller::read()
{
    while(true)
    {
        revolutions = i_revolutions->read();

    }
}

void fuzzy_controller::do_job()
{
    double power;
    while(true)
    {
        /**INPUT*********************************************************/
        /**O numero de rotacoes por segundo em que o motor esta girando */
        /**Esta na variável "revolutions"                               */
        /****************************************************************/
        errorValue = vRef - revolutions;

        errorChangeValue = errorValue - lastError;
        lastError = errorValue;

        errorValue = errorValue*(1.0/vRef);
        errorChangeValue = errorChangeValue*(1.0/(4*vRef));

        error->setInputValue(errorValue);
        changeError->setInputValue(errorChangeValue);
        engine->process();

        power = changePower->defuzzifyNoLocks();
        power = power*(0.5);
        power = power + lastPower;
        lastPower = power;
        cout<<"@" << sc_time_stamp() << " :: POWER " << power <<endl;
        cout << "RPM: " << revolutions << endl;

        /**OUTPUT**************************************************************************************/
        /**///potencia = 255*power*1/(5*5);/**A SAIDA DO ALGORITMO DE VOCES TEM QUE SER ATRIBUIDA À VARIAVEL POTÊNCIA */
        /**/o_power->write(power);/**Coloca na FiFo                                                */
        /**/wait(1, SC_MS);/**Duracao do bloco                                                              */
        /**********************************************************************************************/

    }
}

void fuzzy_controller::initialize()
{
    freq_clock = 1000000;

    engine = new Engine("fuzzy-control");

    error = new InputVariable;
    error->setName("Error");
    error->setRange(-1, 1);
    error->addTerm(new ZShape("NL", -0.5,-0.2));
    error->addTerm(new SShape("PL", 0.2, 0.5));
    error->addTerm(new Triangle("NM", -0.500, -0.100));
    error->addTerm(new Triangle("NS", -0.2, 0.0));
    error->addTerm(new Triangle("Z", -0.100, 0.100));
    error->addTerm(new Triangle("PS", 0.0, 0.2));
    error->addTerm(new Triangle("PM", 0.1, 0.5));
    engine->addInputVariable(error);

    changeError = new InputVariable;
    changeError->setName("ChangeError");
    changeError->setRange(-1, 1);
    changeError->addTerm(new ZShape("NL", -0.5,-0.2));
    changeError->addTerm(new SShape("PL", 0.2, 0.5));
    changeError->addTerm(new Triangle("NM", -0.500, -0.100));
    changeError->addTerm(new Triangle("NS", -0.2, 0.0));
    changeError->addTerm(new Triangle("Z", -0.100, 0.100));
    changeError->addTerm(new Triangle("PS", 0.0, 0.2));
    changeError->addTerm(new Triangle("PM", 0.1, 0.5));
    engine->addInputVariable(changeError);

    changePower = new OutputVariable;
    changePower->setName("ChangePower");
    changePower->setRange(-1, 1);
    changePower->setDefaultValue(fl::nan);
    changePower->addTerm(new ZShape("NL", -0.5,-0.2));
    changePower->addTerm(new SShape("PL", 0.2, 0.5));
    changePower->addTerm(new Triangle("NM", -0.500, -0.100));
    changePower->addTerm(new Triangle("NS", -0.2, 0.0));
    changePower->addTerm(new Triangle("Z", -0.100, 0.100));
    changePower->addTerm(new Triangle("PS", 0.0, 0.2));
    changePower->addTerm(new Triangle("PM", 0.1, 0.5));
    engine->addOutputVariable(changePower);

    ruleblock = new RuleBlock;
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is NL then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is NM then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is NS then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is Z then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is PS then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is PM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is NL and ChangeError is PL then ChangePower is Z", engine));

    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is NL then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is NM then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is NS then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is Z then ChangePower is NM", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is PS then ChangePower is NS", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is PM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is NM and ChangeError is PL then ChangePower is Z", engine));

    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is NL then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is NM then ChangePower is NM", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is NS then ChangePower is NS", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is Z then ChangePower is NS", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is PS then ChangePower is NS", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is PM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is NS and ChangeError is PL then ChangePower is Z", engine));

    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is NL then ChangePower is NL", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is NM then ChangePower is NM", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is NS then ChangePower is NS", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is Z then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is PS then ChangePower is PS", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is PM then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is Z and ChangeError is PL then ChangePower is PL", engine));

    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is NL then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is NM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is NS then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is Z then ChangePower is PS", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is PS then ChangePower is PS", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is PM then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is PS and ChangeError is PL then ChangePower is PL", engine));

    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is NL then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is NM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is NS then ChangePower is PS", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is Z then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is PS then ChangePower is PL", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is PM then ChangePower is PL", engine));
    ruleblock->addRule(Rule::parse("if Error is PM and ChangeError is PL then ChangePower is PL", engine));

    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is NL then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is NM then ChangePower is Z", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is NS then ChangePower is PM", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is Z then ChangePower is PL", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is PS then ChangePower is PL", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is PM then ChangePower is PL", engine));
    ruleblock->addRule(Rule::parse("if Error is PL and ChangeError is PL then ChangePower is PL", engine));

    engine->addRuleBlock(ruleblock);

    //configure(conjunction, disjunction, activation, accumulation, defuzzifier);
    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid");


    if (not engine->isReady(&status))
        throw Exception("Engine not ready. "
            "The following errors were encountered:\n" + status, FL_AT);

}
