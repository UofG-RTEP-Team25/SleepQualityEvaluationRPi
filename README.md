# SleepQualityEvaluationRPi
A implementation of project to use Raspberry Pi based device to detecting and evaluating sleep quality, use C++ mostly.
# How it works
We use several kinds of sensors to monitor users sleep and collect their data related to the sleep. The device, then, evaluate users' sleep quality and diagnose if users suffering from sleep disorders, according to the assessment that given below. Additionally, the diagonsis will be uploaded to a serve eventually and users are allowed to check it out by downloading it through a computer or cellphone. During the sleep, sleep infomation will also be uploaded to the serve in real time，which allows supervisors (maybe baby-sitters, parents, nurses etc.) to record users' real-time sleep state.

Here is a working demonstration of the system:
![15E17EC1-5B44-4358-8A54-BAEF73FF8F33](https://user-images.githubusercontent.com/77977624/109402512-73ac1700-794e-11eb-8671-5e314399f869.jpeg)
# Assessment methodology
According to polysomnography (PSG), we are able to do a sleep study by monitoring or recording bio-physiological changes that occur in the human body when the person is asleep. Based on data collected during sleep, like brain activity, eye movements, muscle activity and heart rhythm, doctors are allowed to diagnose whether patients are suffering from sleep disorders or not.

Although PSG is regarded as a gold rule for sleep research, it also has many drawbacks. For example, people are normally subject to the equipments which are used for collecting data so that they are unlikely to conduct a PSG test at home or even cannot collect reliable data. In addition to this, PSG scoring also usually varies from laboratory to laboratory. Because of those drawbacks, it is obvious that we cannot implement the whole PSG standard on our device directly. Therefore we refer to some other researches, like Sleep Evaluation by Actigraphy and A novel approach using actigraphy to quantify the level of disruption of sleep by in-home ploysomnography etc, and combine those references, then, come up with a assessment.
Here, assessment as follows:

* Sleep latency (20 minutes or less), the moment an individual slept since the lights were switched off.

* Sleep efficiency, which is the minutes of total sleep divided by the minutes spent on bed. In normal cases, it falls between 85%-90%.

* Sleep stages, which are based on the data coming from various kinds of sensors, like sound sensors, light sensors, motion sensors as well as heart rhythm sensors.
# References
[Sleep Evaluation by Actigraphy .pdf](https://github.com/UofG-RTEP-Team25/SleepQualityEvaluationRPi/files/6055883/Sleep.Evaluation.by.Actigraphy.pdf)

[A novel approach using actigraphy to quantify the level of disruption of sleep by in-home polysomnography - the MrOS Sleep Study Sleep disruption by polysomnography.pdf](https://github.com/UofG-RTEP-Team25/SleepQualityEvaluationRPi/files/6055884/A.novel.approach.using.actigraphy.to.quantify.the.level.of.disruption.of.sleep.by.in-home.polysomnography.-.the.MrOS.Sleep.Study.Sleep.disruption.by.polysomnography.pdf)
