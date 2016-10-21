-- pioneer3at.lua
-- Programa Básico para carregar o Pioneer 3-AT
--
-- Petrucio Ricardo Tavares de Medeiros
-- Universidade Federal do Rio Grande do Norte
-- Departamento de Computacao e Automacao Industrial
-- petrucior at gmail (dot) com
--
-- version 1.0
-- October 2016

-- Configurações básicas do modelo
if (sim_call_type==sim_childscriptcall_initialization) then 
    usensors={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    for i=1,16,1 do
        usensors[i]=simGetObjectHandle("Pioneer_p3dx_ultrasonicSensor"..i)
    end
    motorLeft=simGetObjectHandle("Pioneer_p3dx_leftMotor")
    motorRight=simGetObjectHandle("Pioneer_p3dx_rightMotor")
    noDetectionDist=0.5
    maxDetectionDist=0.2
    detect={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    braitenbergL={-0.2,-0.4,-0.6,-0.8,-1,-1.2,-1.4,-1.6, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
    braitenbergR={-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
    v0=2
end 

if (sim_call_type==sim_childscriptcall_cleanup) then 
 
end 

-- Armazenar no vetor detect as informações dos sensores ultrassons
if (sim_call_type==sim_childscriptcall_actuation) then 
    for i=1,16,1 do
        res,dist=simReadProximitySensor(usensors[i])
        if (res>0) and (dist<noDetectionDist) then
            if (dist<maxDetectionDist) then
                dist=maxDetectionDist
            end
            detect[i]=1-((dist-maxDetectionDist)/(noDetectionDist-maxDetectionDist))
        else
            detect[i]=0
        end
    end
    
    -- Definindo valores das velocidade esquerda e direita
    vLeft=1.8
    vRight=2.0
    
    -- Pegando informacoes do pioneer
    pioneer=simGetObjectHandle('Pioneer_p3dx')
    -- Pegando posicao do pioneer
    PosPioneer=simGetObjectPosition(pioneer,-1)
    -- Pegando o x do pioneer
    t=PosPioneer[1]
    
    -- Verifica se chegou na referencia
    if (t > 0.0) then
        vLeft = 0.0
        vRight = 0.0
    end 

    -- Varrer os sensores da frente do pioneer    
    --for i=1,16,1 do
    for i=2,7,1 do
        if ( detect[i] > 0 )then -- Se estiver detectando obstáculo
	    -- Realiza um desvio para o lado oposto
            vLeft=vLeft+braitenbergL[i]*detect[i]
            vRight=vRight+braitenbergR[i]*detect[i]
        end
    end
    -- Envia informações para os atuadores    
    simSetJointTargetVelocity(motorLeft,vLeft)
    simSetJointTargetVelocity(motorRight,vRight)
end 

